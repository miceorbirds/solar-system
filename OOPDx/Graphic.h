#pragma once
#include <iostream>
#include "Object.h"
#include <Windows.h>
#include "DX3DObject.h"
#include "Color.h"
#include "ShaderClass.h"
#include "ModelClass.h"
#include "BallModel.h"
#include "PanelModel.h"
#include  "SpriteFont.h"
#include "SpriteBatch.h"
#include  "SimpleMath.h";
#include "CameraClass.h"
#include "CubeModel.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

class Graphic : public Object
{
public:
	Graphic();
	Graphic(const Graphic& graphic);
	~Graphic();


	void Dispose() override;;

	bool Frame();
	void updateOffset(int,float, float);

	bool Init(int screenWidth, int screenHeight, HWND hwnd);


protected:

	const bool FULL_SCREEN = false;
	const bool VSYNC_ENABLED = true;
	const float SCREEN_DEPTH = 1000.0f;
	const float SCREEN_NEAR = 0.1f;

private:

	bool Render();
	
	bool Init() override;;

private:
	std::unique_ptr <Color> backgroundColor;
	std::unique_ptr<DX3DObject> m_direct;
	std::unique_ptr<ShaderClass> m_ShaderClass;
	std::unique_ptr<CameraClass> m_cameraClass;
	std::unique_ptr<CubeModel> m_cubeModel;

};



