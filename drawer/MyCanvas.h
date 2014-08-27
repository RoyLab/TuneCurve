#pragma once
#include <D3D11.h>
#include <D3DX10math.h>
#include "typedefs.h"
#include "MultiCircle.h"
using namespace GS;

enum AA_MODE;
class D3D11Buffer;
class ShaderLine;

class MyCanvas
{
public:
	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
		D3DXVECTOR3 color;
		D3DXVECTOR2 correction;
	};

public:
	MyCanvas(ID3D11Device *dev,	ID3D11DeviceContext *devCon, HWND hWnd):
	mDev(dev), mDevCon(devCon), mVertexBuffer(nullptr), mWnd(hWnd),
		mIndexBuffer(nullptr), mPixelConstBuffer(nullptr), mShader(nullptr),
		mScreenHeight(0), mScreenWidth(0), mIsPixelBufferChanged(false)
	{
		ZeroMemory(&mPixelConstData, sizeof(mPixelConstData));
	}

	virtual ~MyCanvas(void);

	virtual bool Initialize(int, int);
	virtual bool Frame();
	virtual void Shutdown();
	
	virtual void SetPixelBufferData(const MultiCircle::PixelBufferType &data){mIsPixelBufferChanged = true; mPixelConstData = data;}
	virtual MultiCircle::PixelBufferType *GetPixelBufferData(){mIsPixelBufferChanged = true; return &mPixelConstData;}
	virtual const MultiCircle::PixelBufferType *GetPixelBufferData() const {return &mPixelConstData;}

	ShaderLine *GetShader(){return mShader;}

private:
	ID3D11Device *mDev;
	ID3D11DeviceContext *mDevCon;

	ShaderLine *mShader;
	int mScreenWidth, mScreenHeight;
	
	D3D11Buffer *mVertexBuffer, *mPixelConstBuffer, *mIndexBuffer;
	bool mIsPixelBufferChanged;
	MultiCircle::PixelBufferType mPixelConstData;
	HWND mWnd;
};

