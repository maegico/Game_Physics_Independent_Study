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

	Mesh* mesh = assetManager.GetMesh("cube.obj");
	ColliderMesh* cmesh = assetManager.GetColliderMesh("cube.obj");
	Material* mat = assetManager.GetMaterial("base");
	objs["test"] = (EntityInterface*)new BasicEntity(mesh, cmesh, mat);

	return S_OK;
}

void Game::Update()
{
	camera->update(0.001f);

	collisionManager.CheckCollisions();
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
