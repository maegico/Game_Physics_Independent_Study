//#include "stdafx.h"
#include <Windows.h>
#include "WindowsManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	//WindowsManager win(hInstance, nCmdShow, 1920, 1080, L"Boson Physics");
	WindowsManager win(hInstance, nCmdShow, 1024, 720, L"Boson Physics");

	HRESULT result = E_FAIL;

	result = win.Init();

	if (result)
		return 1;

	return win.Run();
}