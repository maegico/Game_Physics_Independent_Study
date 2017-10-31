#pragma once
#include "GraphicsCore.h"
#include "BasicEntity.h"

class Game : GraphicsCore
{
public:
	Game();
	Game(unsigned int width, unsigned int height, HWND hWnd);
	~Game();

	HRESULT Init();
	void Update();
	void Draw();
	void OnResize(unsigned int width, unsigned int height);
};

