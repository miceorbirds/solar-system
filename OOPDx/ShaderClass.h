#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
using namespace DirectX;
using namespace std;


class ShaderClass
{
private:
	

		struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

public:
	ShaderClass();
	ShaderClass(const ShaderClass&);
	~ShaderClass();
	

		bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int);

private:
	bool InitializeShader(ID3D11Device*, ID3D11DeviceContext*, HWND);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
};

