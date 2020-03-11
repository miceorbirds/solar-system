#pragma once
#include <d3d11.h>
#include <directxmath.h>
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


using namespace DirectX;



class ModelClass
{
protected:

	struct DataOffset
	{
		float x, y;
	};
	struct VERTEX { FLOAT X, Y, Z; Color Color; };


public:
	ModelClass();
	ModelClass(float, float);
	ModelClass(float, float,float,float);
	ModelClass(const ModelClass&);
	
	~ModelClass();
	virtual void ResetPosition();
	virtual void UpdatePositionByDirection();
	virtual bool Initialize(ID3D11Device*, ID3D11DeviceContext*);
	 void Shutdown();
	virtual void Render(ID3D11DeviceContext*, ID3D11Device*);

	int GetIndexCount();

public:
	
	DataOffset pData;
	 float curLoc[2];
	 float dir[2];
	

protected:
	virtual bool InitializeBuffers(ID3D11Device*, ID3D11DeviceContext*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*, ID3D11Device*);

protected:
	
	
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer, * m_colorBuffer, * m_positionBuffer, * m_constBuffer;
	int m_vertexCount, m_indexCount;
	VERTEX vertices[4] = {};
	
	
};