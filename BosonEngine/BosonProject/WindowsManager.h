#pragma once
#include <Windows.h>
#include <iostream>
//#include "stdafx.h"

class WindowsManager
{
public:
	WindowsManager(HINSTANCE hInstance, int nCmdShow, unsigned int width, unsigned int height, std::wstring winTitle);
	~WindowsManager();

	HRESULT Init();

	WPARAM Run();

private:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HINSTANCE hInstance;
	HWND hWnd;
	int nCmdShow;
	unsigned int width;
	unsigned int height;
	std::wstring winClassName;
	std::wstring winTitle;
};

