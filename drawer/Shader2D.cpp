#include "stdafx.h"
#include "Shader2D.h"
#include "D3D11Buffer.h"


Shader2D::Shader2D(void):
	m_matrixBuffer(nullptr)
{
}


Shader2D::~Shader2D(void)
{
	if (m_matrixBuffer) delete m_matrixBuffer;
}

void Shader2D::Release()
{
	if (m_matrixBuffer)
		m_matrixBuffer->ReleaseDeviceBuffer();

	Shader::Release();
}

bool Shader2D::Initialize(ID3D11Device* device, HWND hWnd)
{
	bool res(false);

	res = Shader::Initialize(device, hWnd, L"vs.hlsl", "VMain", L"ps.hlsl", "PMain");
	if (!res) return false;

	m_matrixBuffer = new D3D11Buffer(::Dynamic, ::Const);
	res = m_matrixBuffer->CreateDeviceBuffer(device, nullptr, sizeof(MatrixBufferType));
	if (!res) return false;

	return true;
}

void Shader2D::GetInputLayerout(D3D11_INPUT_ELEMENT_DESC** desc, int& numElements)
{
	numElements = 5;
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

	(*desc)[3].SemanticName = "TOLERANCE";
	(*desc)[3].SemanticIndex = 0;
	(*desc)[3].Format = DXGI_FORMAT_R32_FLOAT;
	(*desc)[3].InputSlot = 0;
	(*desc)[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	(*desc)[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	(*desc)[3].InstanceDataStepRate = 0;

	(*desc)[4].SemanticName = "CORRECTION";
	(*desc)[4].SemanticIndex = 0;
	(*desc)[4].Format = DXGI_FORMAT_R32G32_FLOAT;
	(*desc)[4].InputSlot = 0;
	(*desc)[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	(*desc)[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	(*desc)[4].InstanceDataStepRate = 0;
}

bool Shader2D::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, 
											 D3DXMATRIX projectionMatrix, float scale, bool keepWidth)
{
	HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	ID3D11Buffer *matBufferPtr = m_matrixBuffer->GetDeviceBuffer();
	unsigned int bufferNumber;


	// Transpose the matrices to prepare them for the shader.
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(matBufferPtr, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;
	dataPtr->scale = scale;
	dataPtr->keepWidth = (keepWidth)?0:1;

	// Unlock the constant buffer.
	deviceContext->Unmap(matBufferPtr, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matBufferPtr);

	return true;
}

bool Shader2D::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, 
								D3DXMATRIX projectionMatrix, float scale, bool keepWidth)
{
	bool result;


	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, scale, keepWidth);
	if(!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);

	return true;
}

void Shader2D::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout);

    // Set the vertex and pixel shaders that will be used to render this triangle.
    deviceContext->VSSetShader(m_vertexShader, NULL, 0);
    deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// Render the triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}