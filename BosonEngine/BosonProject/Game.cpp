#include "Game.h"

Game::Game() : GraphicsCore()
{
}

Game::Game(unsigned int width, unsigned int height, HWND hWnd)
	:GraphicsCore(width, height, hWnd)
{
}

Game::~Game()
{
}

HRESULT Game::Init()
{
	HRESULT result = GraphicsCore::InitGraphics();
	if (FAILED(result)) return result;

	return S_OK;
}

void Game::Update()
{
}

void Game::Draw()
{
	GraphicsCore::ClearBackAndDepthBuffers();

	GraphicsCore::Draw();
}

void Game::OnResize(unsigned int width, unsigned int height)
{
	GraphicsCore::OnResize(width, height);
}
