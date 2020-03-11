#pragma once
#include <DirectXCollision.h>

class CameraClass
{
public:
	CameraClass();
	CameraClass(int,int);
	DirectX::XMMATRIX camView;
	DirectX::XMMATRIX camProjection;
	DirectX::XMVECTOR camPosition;
	DirectX::XMVECTOR camTarget;
	DirectX::XMVECTOR camUp;

	DirectX::XMMATRIX getView();
	DirectX::XMMATRIX getProjection();
	DirectX::XMMATRIX getCamViewProjection();
	void rotateCamera();
};

