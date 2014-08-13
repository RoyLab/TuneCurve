#pragma once
#include "MultiCircle.h"

class VirtualDeviceDx11;
class Camera;
class Shader2D;

enum AA_MODE {AA_NONE = 0, AA_MS, AA_GAUSS, AA_STOCHASTIC_GAUSS};


class D3DEngine
{
public:
	D3DEngine(void);
	virtual ~D3DEngine(void);

	bool Initialize(int, int, HWND);
	void Shutdown();

	bool Frame();

	void SetSigmaValue(int val);
	void SetMSAAlvl(int val);
	void SetAAMode(AA_MODE mode);
	void SetClipEdgeValue(int val);
	void SetThetValue(int val);
	void SetNoiseValue(float val);
	AA_MODE GetAAMode()const;

	Camera *GetCamera()const{return mCamera;}
	VirtualDeviceDx11 *GetDevice()const{return mD3D;}
	MultiCircle::PixelBufferType &GetPixelBuffer(){return *(mCircle->GetPixelBufferData());}

	bool mSingleCircle, mKeepWidth;
private:
	VirtualDeviceDx11 *mD3D;
	Camera *mCamera;
	Shader2D *mShader;
	MultiCircle *mCircle;
};

