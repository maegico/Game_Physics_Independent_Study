#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <DirectXMath.h>
#include "Vertex.h"
#include "ThreadManager.h"
#include "Mesh.h"

//should this be a friend class of WindowsManager????

#pragma comment(lib, "d3d11.lib")

class GraphicsCore
{
protected:
	GraphicsCore();
	GraphicsCore(unsigned int width, unsigned int height, HWND hWnd);
	~GraphicsCore();

	HRESULT InitGraphics();
	void Quit();
	virtual void OnResize(unsigned int width, unsigned int height);
	
	virtual HRESULT Init() = 0;
	virtual void Update() = 0;
	virtual void Draw();

	void loadMeshes(std::wstring folderPath);
	void loadMesh(std::string objFile);

	void ClearBackAndDepthBuffers();
	void QueueEntityDraw();
	void QueueEntityBatch();	//not implemented yet

private:
	unsigned int width;
	unsigned int height;
	HWND hWnd;

	D3D_FEATURE_LEVEL featureLvl;
	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* immContext;
	std::vector<ID3D11DeviceContext*> defContexts;
	//ID3D11CommandList* cmdLists[7];

	ID3D11RenderTargetView* backBuffer;
	ID3D11DepthStencilView* depthStencilView;

	ThreadManager* threadManager;

	std::unordered_map<std::string, Mesh*> meshes;
	std::vector<ID3D11CommandList*> cmdLists;
};

