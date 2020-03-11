#pragma once
#include "ModelClass.h"
class CameraClass;

class CubeModel :
	public ModelClass
{
private: struct Vertex    //Overloaded Vertex Structure
{
	Vertex() {}
	Vertex(float x, float y, float z,
		float cr, float cg, float cb, float ca)
		: pos(x, y, z), color(cr, cg, cb, ca) {}

	XMFLOAT3 pos;
	XMFLOAT4 color;
};
	   struct cbPerObject
	   {
		   XMMATRIX  WVP;
	   };
public:
	CubeModel();
	/* void ResetPosition() override;
	 void UpdatePositionByDirection() override;*/
	 //bool Initialize(ID3D11Device*, ID3D11DeviceContext*) override;
	void Shutdown();
	 void Render(ID3D11DeviceContext*, ID3D11Device*, XMMATRIX);

protected:
	bool InitializeBuffers(ID3D11Device*, ID3D11DeviceContext*) override;
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*, ID3D11Device*,XMMATRIX);
	void updateAndDraw(ID3D11DeviceContext*);

protected:
	ID3D11Buffer* cbPerObjectBuffer;
	cbPerObject cbPerObj;
	XMMATRIX cube_world;

	
};

