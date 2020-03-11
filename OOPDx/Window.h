#pragma once

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "Input.h"
#include <memory>
#include "Graphic.h"


class Window;

static Window* ApplicationHandle = nullptr;

class Window : public Object
{
public:

	Window(const wchar_t* name) : applicationName(name)
	{
	};

	Window(const Window& system)
	{
	};

	~Window();
	bool Init() override;
	void Run();
	void Dispose() override;

protected:
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	
private:
	bool Frame();
	void InitialWindow();
	void DisposeWindow();
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
private:

	bool isRequestExit;
	const wchar_t* applicationName;
	int screenWidth, screenHeight;
	HWND m_hwnd;
	std::unique_ptr<Input> m_input;
	std::unique_ptr<Graphic> m_graphic;
	HINSTANCE m_hInstance;
};








