#include "stdafx.h"
#include "MultiCircle.h"
#include <random>

MultiCircle::MultiCircle(ID3D11Device *dev,
						 ID3D11DeviceContext *devCon):
mDev(dev), mDevCon(devCon), mVertexBuff(nullptr),
	mIndexBuff(nullptr), mPixelBuff(nullptr),
	mScreenHeight(0), mScreenWidth(0)
{
	ZeroMemory(&mCirclePara, sizeof(mCirclePara));
	ZeroMemory(&mPixelBufferData, sizeof(mPixelBufferData));
}


MultiCircle::~MultiCircle()
{
	if (mVertexBuff)
	{
		mVertexBuff->ReleaseDeviceBuffer();
		delete mVertexBuff;
		mVertexBuff = nullptr;
	}

	if (mIndexBuff)
	{
		mIndexBuff->ReleaseDeviceBuffer();
		delete mIndexBuff;
		mIndexBuff = nullptr;
	}

	if (mPixelBuff)
	{
		mPixelBuff->ReleaseDeviceBuffer();
		delete mPixelBuff;
		mPixelBuff = nullptr;
	}
}

bool MultiCircle::Initialize(int width, int height)
{
	mScreenWidth = width;
	mScreenHeight = height;
	return InitializeBuffers();
}

bool MultiCircle::Frame()
{
	//pixel buffer upate
	if (mIsPixelBufferChanged)
	{
		mPixelBuff->Update(mDevCon, 0, sizeof(PixelBufferType)/4, &mPixelBufferData);
		ID3D11Buffer *pixelBuff = mPixelBuff->GetDeviceBuffer();
		mDevCon->PSSetConstantBuffers(0, 1, &pixelBuff);

		mIsPixelBufferChanged = false;
	}

	unsigned int stride = sizeof(VertexType); 
	unsigned int offset = 0;

	ID3D11Buffer *verBuff = mVertexBuff->GetDeviceBuffer();

	mDevCon->IASetVertexBuffers(0, 1, &verBuff, &stride, &offset);
	mDevCon->IASetIndexBuffer(mIndexBuff->GetDeviceBuffer(), DXGI_FORMAT_R32_UINT, 0);
	mDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}
void MultiCircle::Shutdown()
{
	if (mVertexBuff)
	{
		mVertexBuff->ReleaseDeviceBuffer();
		delete mVertexBuff;
		mVertexBuff = nullptr;
	}

	if (mIndexBuff)
	{
		mIndexBuff->ReleaseDeviceBuffer();
		delete mIndexBuff;
		mIndexBuff = nullptr;
	}

	if (mPixelBuff)
	{
		mPixelBuff->ReleaseDeviceBuffer();
		delete mPixelBuff;
		mPixelBuff = nullptr;
	}
}

bool MultiCircle::InitializeBuffers()
{
	const int count = 4*mCirclePara.count;

	// Initialize vertex buffer.
	VertexType *vertices = new VertexType[count];

	float size = 1.0f;
	const float step = mCirclePara.maxSize/(mCirclePara.count-1);
	const float correctionFactor = 5.0f;

	if (mCirclePara.count == 1)
	{
		size = mCirclePara.maxSize;
	}

	for (int i = 0; i < mCirclePara.count; i++)
	{
		unsigned int offset = i*4;

		//left, top, right, down
		float4 rect(mCirclePara.center[0]-size,
					mCirclePara.center[1]+size,
					mCirclePara.center[0]+size,
					mCirclePara.center[1]-size);

		//float4 rect(mCirclePara.center[0]-size-correctionFactor,
		//			mCirclePara.center[1]+size+correctionFactor,
		//			mCirclePara.center[0]+size+correctionFactor,
		//			mCirclePara.center[1]-size-correctionFactor);

		float texCoord = 1.0f+correctionFactor/size;
		float color = rand()/float(RAND_MAX);
		float color1 = rand()/float(RAND_MAX);
		float color2 = rand()/float(RAND_MAX);
		// First triangle.
		vertices[offset+0].position = D3DXVECTOR3(rect[0], rect[1], 0.0f);
		vertices[offset+0].texture = D3DXVECTOR2(-texCoord, texCoord);
		vertices[offset+0].correction = D3DXVECTOR2(-correctionFactor, correctionFactor);
		vertices[offset+0].color = D3DXVECTOR3(color, color1, color2);

		vertices[offset+1].position = D3DXVECTOR3(rect[2], rect[3], 0.0f);
		vertices[offset+1].texture = D3DXVECTOR2(texCoord, -texCoord);
		vertices[offset+1].correction = D3DXVECTOR2(correctionFactor, -correctionFactor);
		vertices[offset+1].color = D3DXVECTOR3(color, color1, color2);

		vertices[offset+2].position = D3DXVECTOR3(rect[0], rect[3], 0.0f);
		vertices[offset+2].texture = D3DXVECTOR2(-texCoord, -texCoord);
		vertices[offset+2].correction = D3DXVECTOR2(-correctionFactor, -correctionFactor);
		vertices[offset+2].color = D3DXVECTOR3(color, color1, color2);

		vertices[offset+3].position = D3DXVECTOR3(rect[2], rect[1], 0.0f);
		vertices[offset+3].texture = D3DXVECTOR2(texCoord, texCoord);
		vertices[offset+3].correction = D3DXVECTOR2(correctionFactor, correctionFactor);
		vertices[offset+3].color = D3DXVECTOR3(color, color1, color2);

		vertices[offset+0].tolerance = 
		vertices[offset+1].tolerance = 
		vertices[offset+2].tolerance = 
		vertices[offset+3].tolerance = 1.0f/(rect[1]-rect[3]-correctionFactor*2);

		size += step;
	}

	mVertexBuff = new D3D11Buffer(::Default, ::Vertex);
	mVertexBuff->CreateDeviceBuffer(mDev, vertices, sizeof(VertexType)*count);
	delete [] vertices;
	vertices = nullptr;

	// Initialize index buffer.
	const int indexCount = mCirclePara.count*6;
	unsigned long *indices = new unsigned long[indexCount];
	for (int i = 0; i < mCirclePara.count; i++)
	{
		int stride = 6*i, strideV = 4*i;
		indices[stride+0] = strideV+0;
		indices[stride+1] = strideV+1;
		indices[stride+2] = strideV+2;
		indices[stride+3] = strideV+0;
		indices[stride+4] = strideV+3;
		indices[stride+5] = strideV+1;
	}

	mIndexBuff = new D3D11Buffer(::Default, ::Index);
	mIndexBuff->CreateDeviceBuffer(mDev, indices, sizeof(unsigned long)*indexCount);
	delete [] indices;
	indices = nullptr;

	// Initialize other Buffer.
	mPixelBuff = new D3D11Buffer(::Dynamic, ::Const);
	mPixelBuff->CreateDeviceBuffer(mDev, nullptr, sizeof(PixelBufferType));

	return true;
}