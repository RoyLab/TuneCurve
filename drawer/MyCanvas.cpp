#include "stdafx.h"
#include "MyCanvas.h"
#include "ShaderLine.h"
#include "D3D11Buffer.h"

MyCanvas::~MyCanvas(void)
{
}

bool MyCanvas::Initialize(int width, int height)
{
	mShader = new ShaderLine;
	if (!mShader) return false;

	if (!mShader->Initialize(mDev, mWnd)) return false;

	mScreenWidth = width;
	mScreenHeight = height;

	//left, top, right, down
	float4 rect(-mScreenWidth, mScreenHeight, mScreenWidth, -mScreenHeight);

	VertexType vertices[4];
	vertices[0].position = D3DXVECTOR3(rect[0], rect[1], 0.0f);
	vertices[0].texture = D3DXVECTOR2(-float(mScreenWidth), (float)mScreenHeight);

	vertices[1].position = D3DXVECTOR3(rect[2], rect[3], 0.0f);
	vertices[1].texture = D3DXVECTOR2(float(mScreenWidth), -(float)mScreenHeight);

	vertices[2].position = D3DXVECTOR3(rect[0], rect[3], 0.0f);
	vertices[2].texture = D3DXVECTOR2(-float(mScreenWidth), -(float)mScreenHeight);
		 
	vertices[3].position = D3DXVECTOR3(rect[2], rect[1], 0.0f);
	vertices[3].texture = D3DXVECTOR2(float(mScreenWidth), (float)mScreenHeight);

	mVertexBuffer = new D3D11Buffer(::Default, ::Vertex);
	mVertexBuffer->CreateDeviceBuffer(mDev, vertices, sizeof(VertexType)*4);

	const unsigned int INDEX_COUNT = 6;
	unsigned long *indices = new unsigned long[INDEX_COUNT];

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 3;
	indices[5] = 1;

	mIndexBuffer = new D3D11Buffer(::Default, ::Index);
	mIndexBuffer->CreateDeviceBuffer(mDev, indices, sizeof(unsigned long)*INDEX_COUNT);
	delete [] indices;
	indices = nullptr;

	// Initialize other Buffer.
	mPixelConstBuffer = new D3D11Buffer(::Dynamic, ::Const);
	mPixelConstBuffer->CreateDeviceBuffer(mDev, nullptr, sizeof(MultiCircle::PixelBufferType));

	return true;
}

void MyCanvas::Shutdown()
{
	if (mShader)
	{
		mShader->Release();
		delete mShader;
		mShader = nullptr;
	}
	
	if (mVertexBuffer)
	{
		mVertexBuffer->ReleaseDeviceBuffer();
		delete mVertexBuffer;
		mVertexBuffer = nullptr;
	}

	if (mIndexBuffer)
	{
		mIndexBuffer->ReleaseDeviceBuffer();
		delete mIndexBuffer;
		mIndexBuffer = nullptr;
	}

	if (mPixelConstBuffer)
	{
		mPixelConstBuffer->ReleaseDeviceBuffer();
		delete mPixelConstBuffer;
		mPixelConstBuffer = nullptr;
	}
}


bool MyCanvas::Frame()
{
	//pixel buffer upate
	if (true)
	{
		mPixelConstBuffer->Update(mDevCon, 0, sizeof(MultiCircle::PixelBufferType)/4, &mPixelConstData);
		ID3D11Buffer *pixelBuff = mPixelConstBuffer->GetDeviceBuffer();
		mDevCon->PSSetConstantBuffers(0, 1, &pixelBuff);

		mIsPixelBufferChanged = false;
	}

	unsigned int stride = sizeof(VertexType); 
	unsigned int offset = 0;

	ID3D11Buffer *verBuff = mVertexBuffer->GetDeviceBuffer();

	mDevCon->IASetVertexBuffers(0, 1, &verBuff, &stride, &offset);
	mDevCon->IASetIndexBuffer(mIndexBuffer->GetDeviceBuffer(), DXGI_FORMAT_R32_UINT, 0);
	mDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}