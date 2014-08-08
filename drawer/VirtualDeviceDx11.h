#ifndef _VIRTUAL_DEVICE_DX11_H_
#define _VIRTUAL_DEVICE_DX11_H_


/////////////
// LINKING //
/////////////
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")


//////////////
// INCLUDES //
//////////////
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>
#include "typedefs.h"

using namespace GS;

////////////////////////////////////////////////////////////////////////////////
// Class name: D3DClass
////////////////////////////////////////////////////////////////////////////////
class VirtualDeviceDx11
{	
public:

	VirtualDeviceDx11();
	VirtualDeviceDx11(const VirtualDeviceDx11&);
	virtual ~VirtualDeviceDx11();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();
	
	void Resize (int, int);
	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(D3DXMATRIX&);
	void GetWorldMatrix(D3DXMATRIX&);
	void GetOrthoMatrix(D3DXMATRIX&);

	void GetVideoCardInfo(char*, int&);

	void TurnZBufferOn();
	void TurnZBufferOff();

	void SetScale(float);

protected:
	bool CreateDepthStencilView(int width, int height);

private:

	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;

	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_orthoMatrix;
	float mScale;

	ID3D11DepthStencilState* m_depthDisabledStencilState;
	ID3D11BlendState *m_blendState;

	int2 mScreenSize;
	float2 mClipPlane;
};


#endif