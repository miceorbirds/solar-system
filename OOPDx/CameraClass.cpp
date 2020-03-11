#include "CameraClass.h"

CameraClass::CameraClass()
{
    camPosition = DirectX::XMVectorSet(1.0f, 10.0f, -10.0f, 0.0f);
    camTarget = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    camUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    camView = DirectX::XMMatrixLookAtLH(camPosition, camTarget, camUp);
    camProjection = DirectX::XMMatrixPerspectiveFovLH(0.6f * 3.14f, (float)800 / 800, 0.1f, 1000.0f);
}

CameraClass::CameraClass(int width, int height)
{
    camPosition = DirectX::XMVectorSet(0.0f, 3.0f, -10.0f, 0.0f);
    camTarget = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    camUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    camView = DirectX::XMMatrixLookAtLH(camPosition, camTarget, camUp);
    camProjection = DirectX::XMMatrixPerspectiveFovLH(0.6f * 3.14f, (float)width / height, 0.1f, 1000.0f);
}

DirectX::XMMATRIX CameraClass::getView()
{
    return camView;
}

DirectX::XMMATRIX CameraClass::getProjection()
{
    return camProjection;
}

DirectX::XMMATRIX CameraClass::getCamViewProjection()
{
    return camView * camProjection;
}

void CameraClass::rotateCamera()
{
  
    DirectX::XMVECTOR rotaxis = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(rotaxis, -0.005);
	
    camPosition = DirectX::XMVector4Transform(camPosition, Rotation);
    camView = DirectX::XMMatrixLookAtLH(camPosition, camTarget, camUp);
}


