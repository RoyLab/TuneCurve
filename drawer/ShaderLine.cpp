#include "stdafx.h"
#include "ShaderLine.h"
#include "D3D11Buffer.h"


ShaderLine::ShaderLine(void)
{
}


ShaderLine::~ShaderLine(void)
{
}

void ShaderLine::GetInputLayerout(D3D11_INPUT_ELEMENT_DESC** desc, int& numElements)
{
	numElements = 3;
	*desc = new D3D11_INPUT_ELEMENT_DESC[numElements];

	(*desc)[0].SemanticName = "POSITION";
	(*desc)[0].SemanticIndex = 0;
	(*desc)[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	(*desc)[0].InputSlot = 0;
	(*desc)[0].AlignedByteOffset = 0;
	(*desc)[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	(*desc)[0].InstanceDataStepRate = 0;

	(*desc)[1].SemanticName = "TEXCOORD";
	(*desc)[1].SemanticIndex = 0;
	(*desc)[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	(*desc)[1].InputSlot = 0;
	(*desc)[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	(*desc)[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	(*desc)[1].InstanceDataStepRate = 0;

	(*desc)[2].SemanticName = "COLOR";
	(*desc)[2].SemanticIndex = 0;
	(*desc)[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	(*desc)[2].InputSlot = 0;
	(*desc)[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	(*desc)[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	(*desc)[2].InstanceDataStepRate = 0;
}

bool ShaderLine::Initialize(ID3D11Device* device, HWND hWnd)
{
	bool res(false);

	res = Shader::Initialize(device, hWnd, L"vsCanvas.hlsl", "main", L"psCanvas.hlsl", "main");
	if (!res) return false;

	m_matrixBuffer = new D3D11Buffer(::Dynamic, ::Const);
	res = m_matrixBuffer->CreateDeviceBuffer(device, nullptr, sizeof(MatrixBufferType));
	if (!res) return false;

	return true;
}