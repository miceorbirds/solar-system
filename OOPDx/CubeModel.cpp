#include "CubeModel.h"

CubeModel::CubeModel()
{
}

void CubeModel::Render(ID3D11DeviceContext* deviceContext, ID3D11Device* device, XMMATRIX camVP)
{
    RenderBuffers(deviceContext, device,camVP);
}

bool CubeModel::InitializeBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{

    Vertex v[] =
    {
        Vertex(-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f),
        Vertex(-0.5f, +0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f),
        Vertex(+0.5f, +0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f),
        Vertex(+0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f),
        Vertex(-0.5f, -0.5f, +0.5f, 0.0f, 1.0f, 1.0f, 1.0f),
        Vertex(-0.5f, +0.5f, +0.5f, 1.0f, 1.0f, 1.0f, 1.0f),
        Vertex(+0.5f, +0.5f, +0.5f, 1.0f, 0.0f, 1.0f, 1.0f),
        Vertex(+0.5f, -0.5f, +0.5f, 1.0f, 0.0f, 0.0f, 1.0f),
    };

    DWORD indices[] = {
        // front face
        0, 1, 2,
        0, 2, 3,

        // back face
        4, 6, 5,
        4, 7, 6,

        // left face
        4, 5, 1,
        4, 1, 0,

        // right face
        3, 2, 6,
        3, 6, 7,

        // top face
        1, 5, 6,
        1, 6, 2,

        // bottom face
        4, 0, 3,
        4, 3, 7
    };

    D3D11_BUFFER_DESC indexBufferDesc;
    ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(DWORD) * 12 * 3;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA iinitData;

    iinitData.pSysMem = indices;
    device->CreateBuffer(&indexBufferDesc, &iinitData, &m_indexBuffer);

    deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);


    D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * 8;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA vertexBufferData;

    ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
    vertexBufferData.pSysMem = v;
    device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_vertexBuffer);
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    D3D11_BUFFER_DESC cbbd;
    ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

    cbbd.Usage = D3D11_USAGE_DEFAULT;
    cbbd.ByteWidth = sizeof(cbPerObject);
	
    cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbbd.CPUAccessFlags = 0;
    cbbd.MiscFlags = 0;

    device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);

    return true;
}

void CubeModel::RenderBuffers(ID3D11DeviceContext* deviceContext, ID3D11Device* device, XMMATRIX camVP)
{
   
    static float rot;
    rot += .005f;
    if (rot > 6.26f)
        rot = 0.0f;

    static float rotSelf;
    rotSelf += .08f;
    if (rotSelf > 6.26f)
        rotSelf = 0.0f;

    float x;
    
    XMVECTOR rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMMATRIX Rotation = XMMatrixRotationAxis(rotaxis, rot);
    XMMATRIX RotationSelf = XMMatrixRotationRollPitchYaw(0, rotSelf,0);
    XMMATRIX  Translation = XMMatrixTranslation(0.0f, 5.0f, 2.0f);
	for(float i =3;i<=8;i+=0.5)
    {
		
        Rotation = XMMatrixRotationAxis(rotaxis, rot+i);
        Translation = XMMatrixTranslation(0.0f+i, 0.0f+i, 0.0f);
		if(std::modf(i, &x)>0) cube_world = RotationSelf * Translation * Rotation;
        else cube_world =  Translation * Rotation;

  
        cbPerObj.WVP = cube_world * camVP;
        updateAndDraw(deviceContext);
		
	}
    
   
  
	
}

void CubeModel::updateAndDraw(ID3D11DeviceContext* deviceContext)
{
    deviceContext->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    deviceContext->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
    deviceContext->DrawIndexed(36, 0, 0);
}
