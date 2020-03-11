#include "ShaderClass.h"
#include <iostream>

ShaderClass::ShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
}

ShaderClass::ShaderClass(const ShaderClass&)
{
}

ShaderClass::~ShaderClass()
{
}

bool ShaderClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device,deviceContext, hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount)
{
	RenderShader(deviceContext, indexCount);
	return true;
}

bool ShaderClass::InitializeShader(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd)
{
	HRESULT res;
	ID3D10Blob* errorVertexCode;
	ID3D10Blob* errorPixelCode;
	ID3D10Blob* vertexBC;
	ID3D10Blob* pixelBC;
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	errorVertexCode = 0;
	errorPixelCode = 0;
	vertexBC = 0;
	pixelBC = 0;
	res = D3DCompileFromFile(L"MiniTri.fx",
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexBC,
		&errorVertexCode);

	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode) {
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, L"MiniTri.fx", L"Missing Shader File", MB_OK);
		}

		return 0;
	}

	res = D3DCompileFromFile(L"MiniTri.fx", nullptr /*macros*/, nullptr /*include*/, "PSMain", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelBC, &errorPixelCode);

	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorPixelCode) {
			char* compileErrors = (char*)(errorPixelCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, L"MiniTri.fx", L"Missing Shader File", MB_OK);
		}

		return 0;
	}


	res =  device->CreateVertexShader(
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		nullptr, &m_vertexShader);
	
	if (FAILED(res))
	{
		return false;
	}
	
	res = device->CreatePixelShader(
		pixelBC->GetBufferPointer(),
		pixelBC->GetBufferSize(),
		nullptr, &m_pixelShader);
	
	if (FAILED(res))
	{
		return false;
	}
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};



	// Create the vertex input layout.
	res = device->CreateInputLayout(layout, 2, vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(), &m_layout);

	deviceContext->IASetInputLayout(m_layout);
	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexBC->Release();
	vertexBC = 0;
	
	pixelBC->Release();
	pixelBC = 0;

	return true;
	
}

void ShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	
	//deviceContext->DrawIndexed(6, 0, 0);
}
