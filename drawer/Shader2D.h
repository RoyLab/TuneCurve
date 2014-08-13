#pragma once
#include "shader.h"
#include <D3DX10math.h>

class D3D11Buffer;

class Shader2D :
	public Shader
{
public:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
		float scale;
		int keepWidth; 
	};

public:
	Shader2D(void);
	virtual ~Shader2D(void);
	
	bool Initialize(ID3D11Device* device, HWND hWnd);
	void Release();

	bool Render(ID3D11DeviceContext* deviceContext, int indexCount,
		D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, float scale, bool KeepWidth);
protected:
	virtual void GetInputLayerout(D3D11_INPUT_ELEMENT_DESC**, int& );
	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, float, bool);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

protected:
	D3D11Buffer* m_matrixBuffer;
};

