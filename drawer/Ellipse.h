#pragma once
#include "D3D11Buffer.h"
#include "typedefs.h"
#include "D3DEngine.h"
#include <D3DX10math.h>
#include <D3D11.h>
using namespace GS;

class MyEllipse
{
public:
	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
		D3DXVECTOR3 color;
		float tolerance;
		D3DXVECTOR2 correction;
	};

	struct PixelBufferType
	{
		 D3DXVECTOR4 coef;
		 float C;
		 float width;
		 AA_MODE AAMode;
	};

	struct CirclePara
	{
		int count;
		float2 center;
		float maxSize;
	};
public:
	MyEllipse(ID3D11Device*, ID3D11DeviceContext*);
	virtual ~MyEllipse();

	virtual bool Initialize(int, int);
	virtual bool Frame();
	virtual void Shutdown();

	virtual void SetCircleParameter(const CirclePara &para)	{mCirclePara = para;}
	virtual CirclePara *GetCircleParameter(){return &mCirclePara;}

	virtual int GetIndexCount() const {return mCirclePara.count*6;}

	virtual void SetPixelBufferData(const PixelBufferType &data){mIsPixelBufferChanged = true; mPixelBufferData = data;}
	virtual PixelBufferType *GetPixelBufferData(){mIsPixelBufferChanged = true; return &mPixelBufferData;}
	virtual const PixelBufferType *GetPixelBufferData() const {return &mPixelBufferData;}

protected:
	virtual bool InitializeBuffers();

protected:
	ID3D11Device *mDev;
	ID3D11DeviceContext *mDevCon;
	int mScreenWidth, mScreenHeight;

	D3D11Buffer *mVertexBuff, *mIndexBuff, *mPixelBuff;
	CirclePara mCirclePara;
	PixelBufferType mPixelBufferData;
	AA_MODE mAAMode;

	bool mIsPixelBufferChanged;
};

