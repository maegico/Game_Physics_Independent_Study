#pragma once
#include "GraphicsCore.h"
#include "BasicEntity.h"
#include "Collision.h"

class Game : GraphicsCore
{
public:
	Collision::CollisionSystem collisionManager;

	Game();
	Game(unsigned int width, unsigned int height, HWND hWnd);
	~Game();

	HRESULT Init();
	void Update();
	void Draw();
	void OnResize(unsigned int width, unsigned int height);
};

