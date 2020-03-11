#pragma once
#include "Object.h"
#include "Color.h"
#include <Windows.h>
#include <iostream>
#include "winuser.h"
#include <wrl.h>


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include <d3d.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <DirectXColors.h>
#include <dcomp.h>
#include <d3d11.h>


struct Color;

class DX3DObject :	public Object
{
	
public:

	DX3DObject();

	DX3DObject(const DX3DObject& dx_3d_object) {  }

	~DX3DObject() { Dispose(); }

	bool Init(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen,
		float screenDepth, float screenNear);
	void Dispose() override;

	void BeginScene(Color);
	void EndScene();

	ID3D11Device* GetDevice() const;

	ID3D11DeviceContext* GetDeviceContext() const;

	void GetVideoCardInfo(char* cardName, int& memory) const;

	

protected:
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

	
private:

	
	
	bool Init() override;
};



