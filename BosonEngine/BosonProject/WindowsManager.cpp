#include "WindowsManager.h"

WindowsManager::WindowsManager(HINSTANCE hInstance, int nCmdShow, unsigned int width, unsigned int height, std::wstring winTitle)
	: hInstance(hInstance), nCmdShow(nCmdShow), width(width), height(height), winTitle(winTitle)
{
	winClassName = L"WinClass";
}


WindowsManager::~WindowsManager()
{
}

HRESULT WindowsManager::Init()
{
	WNDCLASS winClass;	//struct containing info about window
	ZeroMemory(&winClass, sizeof(WNDCLASS));	//zero out memory in the struct

	//ignoring cbClsExtra and cbWndExtra
	winClass.style = CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc = WindowProc;
	winClass.hInstance = WindowsManager::hInstance;
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	winClass.lpszMenuName = NULL;
	winClass.lpszClassName = winClassName.c_str();

	//register the window class
	if (!RegisterClass(&winClass))
	{
		DWORD err = GetLastError();

		if (err != ERROR_CLASS_ALREADY_EXISTS)
			return HRESULT_FROM_WIN32(err);
	}

	RECT clientRectangle;
	SetRect(&clientRectangle, 0, 0, width, height);
	AdjustWindowRect(&clientRectangle, WS_OVERLAPPEDWINDOW, false);

	RECT desktopRectangle;
	GetClientRect(GetDesktopWindow(), &desktopRectangle);
	int xCenter = (desktopRectangle.right / 2) - (clientRectangle.right / 2);
	int yCenter = (desktopRectangle.bottom / 2) - (clientRectangle.bottom / 2);

	this->hWnd = CreateWindowEx(NULL,
		winClassName.c_str(),
		winTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		xCenter, yCenter, clientRectangle.right-clientRectangle.left, clientRectangle.bottom - clientRectangle.top,
		NULL, NULL,
		this->hInstance, NULL);

	if (hWnd == NULL)
	{
		DWORD err = GetLastError();
		return HRESULT_FROM_WIN32(err);
	}

	ShowWindow(hWnd, nCmdShow);

	return S_OK;
}

WPARAM WindowsManager::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	bool stop = false;

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);

			DispatchMessage(&msg);
		}
		else
		{
			//run stuff here
		}
	}

	return msg.wParam;
}

LRESULT WindowsManager::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT WindowsManager::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//need to find a better method for handling various keys

	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CHAR:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
			return 0;
		}
		break;
	case WM_SIZE:
		width = LOWORD(lParam);
		height = HIWORD(lParam);
	case WM_MENUCHAR:
		return MAKELRESULT(0, MNC_CLOSE);
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
