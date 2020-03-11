#include "Graphic.h"
#include "ModelClass.h"

Graphic::Graphic() {};

Graphic::Graphic(const Graphic& graphic) {};

Graphic::~Graphic()
{
	Dispose();	
}

void Graphic::Dispose()
{
	if(!isInited)return;

	if(m_direct)
	{
		m_direct->Dispose();
		m_direct.reset();
		m_direct = nullptr;

		std::cout << "Destroy " << typeid(this).name() << " Class" << std::endl;
	}

	isInited = false;
}

bool Graphic::Frame()
{
	if (!isInited)return isInited;
	return Render();
}

void Graphic::updateOffset(int player,float x, float y)
{
	/*if (player == 0)
	{
		m_modelLeftPanel->updatePositionByClick(x, y);
	}
	if(player==1)
	{
		m_modelRightPanel->updatePositionByClick(x, y);
	}*/

}

bool Graphic::Init(int screenWidth, int screenHeight, HWND hwnd)
{
	m_direct = std::make_unique<DX3DObject>();
	if(!m_direct)
	{
		return false;
	}

	bool result = m_direct->Init(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	
	if(!result)
	{
		MessageBox(hwnd, L"DirectX initial failed", L"Error", MB_OK);
		return result;
	}
	else
	{
		result = Init();
	}

	m_cubeModel = std::make_unique<CubeModel>();
	m_cubeModel->Initialize(m_direct->GetDevice(),m_direct->GetDeviceContext());
	
	m_ShaderClass = std::make_unique<ShaderClass>();

	bool res = m_ShaderClass->Initialize(m_direct->GetDevice(),m_direct->GetDeviceContext(), hwnd);


	m_cameraClass = std::make_unique<CameraClass>();
	
	return result;
}

bool Graphic::Render()
{

	
	bool result;
	m_direct->BeginScene(Green);
	m_cameraClass->rotateCamera();
	m_cubeModel->Render(m_direct->GetDeviceContext(), m_direct->GetDevice(), m_cameraClass->getCamViewProjection());


	m_direct->EndScene();
	
	return true;
}

bool Graphic::Init()
{
	std::cout << "Init " << typeid(this).name() << " Class\n";
	backgroundColor = std::make_unique<Color>(1,0,0,1);
	isInited = true;
	return isInited;
}


