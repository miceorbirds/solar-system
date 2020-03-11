#include "windows.h"
#include "Window.h"

int WINAPI main(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	auto system = std::make_unique<Window>(L"Game");
	
	if(!system)
	{
		return 0;
	}

	if(system->Init())
	{
		system->Run();
	}
	system->Dispose();

	return  0;
}
