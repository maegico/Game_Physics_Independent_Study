#include "GraphicsCore.h"



GraphicsCore::GraphicsCore(unsigned int width, unsigned int height, HWND hWnd)
	: width(width), height(height), hWnd(hWnd)
{
	device = nullptr;
	context = nullptr;
	swapChain = nullptr;
	backBuffer = nullptr;
	depthStencilView = nullptr;
}


GraphicsCore::~GraphicsCore()
{
	if (depthStencilView) { depthStencilView->Release(); }
	if (backBuffer) { backBuffer->Release(); }

	if (swapChain) { swapChain->Release(); }
	if (context) { context->Release(); }
	if (device) { device->Release(); }
}

HRESULT GraphicsCore::InitGraphics()
{
	unsigned int deviceFlags = 0;

#if defined(DEBUG) | defined(_DEBUG)
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.Flags = 0;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = true;

	return E_NOTIMPL;
}
