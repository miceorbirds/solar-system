#include "Window.h"

Window::~Window()
{
	Dispose();	
}

bool Window::Init()
{
	screenWidth = 0, screenHeight = 0;
	
	InitialWindow();
	
	m_input = std::make_unique<Input>();
	if(m_input)
		m_input->Init();
	else
		return false;

	m_graphic = std::make_unique<Graphic>();
	if(m_graphic)
		m_graphic->Init(screenWidth, screenHeight, m_hwnd);
	else
		return false;

	isInited = true;
	std::cout << "Init " << typeid(this).name() << " Class" << std::endl;
	
	return isInited;
}

void Window::Run()
{
	if(!isInited) return;
	
	MSG message{};

	ZeroMemory(&message, sizeof(MSG));

	while (!isRequestExit)
	{
		if (PeekMessage(&message, m_hwnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);			
			DispatchMessage(&message);
		}		
		
		if (message.message == WM_QUIT)
		{
			isRequestExit = true;
		}
		else
		{
			
		}
	}
}

void Window::Dispose()
{
	if(!isInited)		return;
	
	m_input->Dispose();
	m_input.reset();
	m_input = nullptr;

	m_graphic->Dispose();
	m_graphic.reset();
	m_graphic = nullptr;

	DisposeWindow();

	std::cout << "Destroy " << typeid(this).name() << " Class" << std::endl;

	isInited = false;
}


void Window::InitialWindow()
{
	{
		WNDCLASSEX wc;
		DEVMODE dmScreenSettings;
		int posX, posY;

		ApplicationHandle = this;

		m_hInstance = GetModuleHandle(nullptr);
		
		/// Setup the windows class with default settings.
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hIconSm = wc.hIcon;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = applicationName;
		wc.cbSize = sizeof(WNDCLASSEX);

		// Register the window class.
		RegisterClassEx(&wc);

		// Determine the resolution of the clients desktop screen.
		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);

		// If windowed then set it to 800x800 resolution.
		screenWidth = 800;
		screenHeight = 800;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

		RECT windowRect = { 0, 0, static_cast<LONG>(screenWidth), static_cast<LONG>(screenHeight) };
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

		auto dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME;
		// Create the window with the screen settings and get the handle to it.
		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, applicationName, applicationName,
			dwStyle,
			posX, posY,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			NULL, NULL, m_hInstance, nullptr);

		ShowWindow(m_hwnd, SW_SHOW);
		SetForegroundWindow(m_hwnd);
		SetFocus(m_hwnd);
		ShowCursor(true);
	}
}

void Window::DisposeWindow()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = nullptr;

	// Remove the application instance.
	UnregisterClass(applicationName, m_hInstance);
	m_hInstance = nullptr;

	// Release the pointer to this class.
	ApplicationHandle = nullptr;
}


bool Window::Frame()
{
	if(m_input->IsKeyPressed(VK_ESCAPE))
	{
		return false;
	}

	if (m_input->IsKeyPressed(87)) m_graphic->updateOffset(0,0,0.015);
	if (m_input->IsKeyPressed(83)) m_graphic->updateOffset(0,0,-0.015);
	if (m_input->IsKeyPressed(38)) m_graphic->updateOffset(1, 0, 0.015);
	if (m_input->IsKeyPressed(40)) m_graphic->updateOffset(1, 0, -0.015);
	
	return m_graphic->Frame();	
}


LRESULT CALLBACK Window::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
	case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if a key has been pressed on the keyboard.

	case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			m_input->KeyDown((unsigned int)wparam);
			return 0;
		}

	


	case WM_PAINT:
		{
			const bool isCorrect = Frame();
			if (!isCorrect)
			{
				isRequestExit = true;
			}

			return 0;
		}

		// Check if a key has been released on the keyboard.
	case WM_KEYUP:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			m_input->KeyUp((unsigned int)wparam);
			return 0;
		}

		// Any other messages send to the default message handler as our application won't make use of them.
	default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
}
