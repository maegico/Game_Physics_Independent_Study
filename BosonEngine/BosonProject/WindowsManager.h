#pragma once
#include <Windows.h>
#include <iostream>
#include "Game.h"

class WindowsManager
{
public:
	WindowsManager(HINSTANCE hInstance, int nCmdShow, unsigned int width, unsigned int height, std::wstring winTitle);
	~WindowsManager();

	static WindowsManager* wndManagerInstance;
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HRESULT Init();

	WPARAM Run();

private:
	HINSTANCE hInstance;
	HWND hWnd;
	int nCmdShow;
	unsigned int width;
	unsigned int height;
	std::wstring winClassName;
	std::wstring winTitle;
	Game* game;

	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

