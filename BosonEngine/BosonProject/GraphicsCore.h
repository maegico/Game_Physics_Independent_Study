#pragma once
#include <d3d11.h>
#include <Windows.h>

//should this be a friend class of WindowsManager????

class GraphicsCore
{
public:
	GraphicsCore(unsigned int width, unsigned int height, HWND hWnd);
	~GraphicsCore();

	HRESULT InitGraphics();

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

