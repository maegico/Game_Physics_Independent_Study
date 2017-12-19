#include "Game.h"

Game::Game() : GraphicsCore()
{
	printf("Testing output.\n");
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

	std::string meshName = "sphere.obj";
	Mesh* mesh = assetManager.GetMesh(meshName.c_str());
	ColliderMesh cmesh = assetManager.GetColliderMesh(meshName.c_str());
	Material* mat = assetManager.GetMaterial("base");

	std::string meshName2 = "sphere2.obj";
	Mesh* mesh2 = assetManager.GetMesh(meshName2.c_str());
	ColliderMesh cmesh2 = assetManager.GetColliderMesh(meshName2.c_str());
	Material* mat2 = assetManager.GetMaterial("softbody");

	objs["test1"] = (EntityInterface*)new BasicEntity(mesh, cmesh, mat);
	objs["test2"] = (EntityInterface*)new BasicEntity(mesh2, cmesh2, mat2);

	objs["test1"]->transform.position = DirectX::XMFLOAT3(2, 0, 0);
	objs["test2"]->transform.position = DirectX::XMFLOAT3(-2, 0, 0);

	result = GraphicsCore::Init();
	if (FAILED(result)) return result;

	collisionManager.BatchCollider(objs["test1"]->collider);
	collisionManager.BatchCollider(objs["test2"]->collider);

	return S_OK;
}

void Game::Update()
{
	//now does the update based on forces
	if (GetAsyncKeyState('K') & 0x8000) { objs["test1"]->transform.force.x += -0.600f; }
	if (GetAsyncKeyState('L') & 0x8000) { objs["test1"]->transform.force.x += 0.600f; }

	//Update all game objs and other graphics related values
	GraphicsCore::Update();

	//Check Collisions
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
