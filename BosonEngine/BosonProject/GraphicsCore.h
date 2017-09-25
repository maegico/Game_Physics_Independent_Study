#pragma once
#include <Windows.h>
#include <d3d11.h>

//should this be a friend class of WindowsManager????

#pragma comment(lib, "d3d11.lib")

class GraphicsCore
{
public:
	GraphicsCore();
	GraphicsCore(unsigned int width, unsigned int height, HWND hWnd);
	~GraphicsCore();

	HRESULT InitGraphics();
	void Quit();
	void ClearBackAndDepthBuffers();
	virtual void OnResize(unsigned int width, unsigned int height);
	
	virtual HRESULT Init() = 0;
	virtual void Update() = 0;
	virtual void Draw();

private:
	unsigned int width;
	unsigned int height;
	HWND hWnd;

	D3D_FEATURE_LEVEL featureLvl;
	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* context;

	ID3D11RenderTargetView* backBuffer;
	ID3D11DepthStencilView* depthStencilView;
};

