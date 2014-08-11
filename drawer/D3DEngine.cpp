#include "stdafx.h"
#include "D3DEngine.h"
#include "VirtualDeviceDx11.h"
#include "Camera.h"
#include "Shader2D.h"
#include "Ellipse.h"


D3DEngine::D3DEngine(void):
	mD3D(nullptr), mCamera(nullptr), 
	mShader(nullptr), mCircle(nullptr)
{
}


D3DEngine::~D3DEngine(void)
{
}

bool D3DEngine::Initialize(int width, int height, HWND hWnd)
{
	bool res = false;

	mD3D = new VirtualDeviceDx11;
	res = mD3D->Initialize(width, height, true, hWnd, false, 1000.0f, 0.1f);
	if (!res) return false;

	mCamera = new Camera;
	if(!mCamera) return false;
	mCamera->SetPosition(0.0f, 0.0f, -10.0f);

	mShader = new Shader2D;
	if(!mShader)
		return false;

	// Initialize the texture shader object.
	res = mShader->Initialize(mD3D->GetDevice(), hWnd);
	if(!res)
	{
		MessageBox(hWnd, L"Could not initialize the shader object.", L"Error", MB_OK);
		return false;
	}

	//mCircle = new Circle;
	mCircle = new MyEllipse(mD3D->GetDevice(), mD3D->GetDeviceContext());
	if (!mCircle) return false;

	//res = mCircle->Initialize(mD3D->GetDevice(), width, height);
	MyEllipse::CirclePara cPara;
	cPara.center = float2(0, 0);
	cPara.maxSize = 250.0f;
	cPara.count = 1;
	mCircle->SetCircleParameter(cPara);

	MyEllipse::PixelBufferType pBufferData;
	pBufferData.coef[0] = 1.0f;
	pBufferData.coef[1] = 1.0f;
	pBufferData.coef[2] = 0.0f;
	pBufferData.coef[3] = 0.0f;
	pBufferData.C = -40000.0f;
	pBufferData.width = 2.0f;
	pBufferData.AAMode = AA_GAUSS;
	mCircle->SetPixelBufferData(pBufferData);

	res = mCircle->Initialize(width, height);
	if(!res)
	{
		MessageBox(hWnd, L"Could not initialize the circle object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void D3DEngine::Shutdown()
{
	if (mCircle)
	{
		mCircle->Shutdown();
		delete mCircle;
		mCircle = nullptr;
	}

	if (mCamera)
	{
		delete mCamera;
		mCamera = nullptr;
	}

	if (mShader)
	{
		mShader->Release();
		delete mShader;
		mShader = nullptr;
	}

	if (mD3D)
	{
		mD3D->Shutdown();
		delete mD3D;
		mD3D = nullptr;
	}
}

bool D3DEngine::Frame()
{
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
	bool res(false);

	mD3D->BeginScene(0.1f, 0.1f, 0.2f, 1.0f);
	mCamera->Render();

	mCamera->GetViewMatrix(viewMatrix);
	mD3D->GetWorldMatrix(worldMatrix);
	mD3D->GetOrthoMatrix(orthoMatrix);

	mD3D->TurnZBufferOff();

	//res = mCircle->Render(mD3D->GetDeviceContext(), 100, 100, 256, 256, mSigma, mMSAAlvl);
	res = mCircle->Frame();
	if (!res) return false;

	res = mShader->Render(mD3D->GetDeviceContext(), mCircle->GetIndexCount(), 
		worldMatrix, viewMatrix, orthoMatrix, mCamera->GetScale());
	if (!res) return false;

	mD3D->EndScene();

	return true;
}

void D3DEngine::SetAAMode(AA_MODE mode)
{
	mCircle->GetPixelBufferData()->AAMode = mode;
}

AA_MODE D3DEngine::GetAAMode()const
{
	return mCircle->GetPixelBufferData()->AAMode;
}

void D3DEngine::SetSigmaValue(int val)
{
	float tmp = (float)2.0e-2*val;
	//mCircle->GetPixelBufferData()->sigma_2 = -2.0*tmp*tmp;
}

void D3DEngine::SetThetValue(int val)
{
	//mCircle->GetPixelBufferData()->thet = (float)val/100.0f+1.0f;
}

void D3DEngine::SetMSAAlvl(int val)
{
	//mCircle->GetPixelBufferData()->samplelvl = val;
}

void D3DEngine::SetClipEdgeValue(int val)
{
	//mCircle->GetPixelBufferData()->clipEdge = (float)val/20.0f+1.0f;
}

void D3DEngine::SetNoiseValue(float val)
{
	//mCircle->GetPixelBufferData()->noise = int(val*200);
}