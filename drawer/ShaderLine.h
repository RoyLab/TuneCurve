#pragma once
#include "shader2d.h"
class ShaderLine :
	public Shader2D
{
public:
	ShaderLine(void);
	virtual ~ShaderLine(void);

	bool Initialize(ID3D11Device* device, HWND hWnd);
	void GetInputLayerout(D3D11_INPUT_ELEMENT_DESC** desc, int& numElements);
};

