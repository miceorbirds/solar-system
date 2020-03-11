#include "ModelClass.h"
#include <xutility>

#define ZCHECK(exp) if(FAILED(exp)) { printf("Check failed at file: %s at line %i", __FILE__, __LINE__); return false; }

ModelClass::ModelClass() {

	
	curLoc[0] = 0;
	curLoc[1] = 0;
	dir[0] = 0;
	dir[1] = 0;
	vertices[0] = { 0.03f, 0.03f, 0.0f, Color(1.0f, 1.0f, 1.0f, 1.0f) };
	vertices[1] = { 0.03f, -0.03, 0.0f, Color(1.0f, 1.0f, 1.0f, 1.0f) };
	vertices[2] = { -0.03f, -0.03f, 0.0f, Color(1.0f, 1.0f, 1.0f, 1.0f) };
	vertices[3] = { -0.03f, 0.03f, 0.0f, Color(1.0f, 1.0f, 1.0f, 1.0f) };

	
}

ModelClass::ModelClass(float x, float y)
{
	dir[0] = x;
	dir[1] = y;
	ResetPosition();

}

ModelClass::ModelClass(float x, float y, float x_offset, float y_offset)
{
	curLoc[0] = x_offset;
	curLoc[1] = y_offset;
	dir[0] = x;
	dir[1] = y;
	vertices[0] = { 0.03f, 0.03f, 0.0f, Color(1.0f, 1.0f, 1.0f, 1.0f) };
	vertices[1] = { 0.03f, -0.03, 0.0f, Color(1.0f, 1.0f, 1.0f, 1.0f) };
	vertices[2] = { -0.03f, -0.03f, 0.0f, Color(1.0f, 1.0f, 1.0f, 1.0f) };
	vertices[3] = { -0.03f, 0.03f, 0.0f, Color(1.0f, 1.0f, 1.0f, 1.0f) };

	for (auto i = 0; i < 4; i++) {
		vertices[i].X += x_offset;
		vertices[i].Y += y_offset;
	}
}


ModelClass::~ModelClass()
{
}

void ModelClass::ResetPosition()
{
	float r = ((float)(rand() % 10 + 1) - 5.0) / 10;
	curLoc[0] = 0;
	curLoc[1] = r;
	vertices[0] = { 0.03f, 0.03f, 0.0f, Color(1.0f, 1.0f, 1.0f, 1.0f) };
	vertices[1] = { 0.03f, -0.03, 0.0f, Color(1.0f, 1.0f, 1.0f, 1.0f) };
	vertices[2] = { -0.03f, -0.03f, 0.0f, Color(1.0f, 1.0f, 1.0f, 1.0f) };
	vertices[3] = { -0.03f, 0.03f, 0.0f, Color(1.0f, 1.0f, 1.0f, 1.0f) };
	for (auto i = 0; i < 4; i++) {
		vertices[i].Y += r;
	}
}

void ModelClass::UpdatePositionByDirection()
{
	curLoc[0] += dir[0] * 0.01;
	curLoc[1] += dir[1] * 0.01;
	if (std::abs(curLoc[0]) >= 1)
	{
		dir[0] = -dir[0];
		//ResetPosition();
	}
	if (std::abs(curLoc[1]) >= 1)
	{
		dir[1] = -dir[1];
	}
	for (auto i = 0; i < 4; i++) {
		vertices[i].X += dir[0] * 0.01;
		vertices[i].Y += dir[1] * 0.01;
	}
}

bool ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
	bool result;


	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device,context);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::Render(ID3D11DeviceContext* context, ID3D11Device* device)
{
	RenderBuffers(context,device);
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

bool ModelClass::InitializeBuffers(ID3D11Device* device, ID3D11DeviceContext* context)
{
	HRESULT res;
	// Set the number of indices in the index array.
	m_indexCount =6;

	int indices[] = { 0,1,2, 2,3,0 };
	

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(int) * std::size(indices);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	iinitData.SysMemPitch = 0;
	iinitData.SysMemSlicePitch = 0;
	device->CreateBuffer(&indexBufferDesc, &iinitData, &m_indexBuffer);
	context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VERTEX) * 4;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = vertices;
	 device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_vertexBuffer);
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

return true;

}

void ModelClass::RenderBuffers(ID3D11DeviceContext* context, ID3D11Device* device)
{

	this->UpdatePositionByDirection();
	this->InitializeBuffers(device, context); 
	context->DrawIndexed(m_indexCount, 0, 0);
	
}
