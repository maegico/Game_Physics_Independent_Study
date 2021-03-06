#include "GraphicsCore.h"

GraphicsCore::GraphicsCore()
{
}

GraphicsCore::GraphicsCore(unsigned int width, unsigned int height, HWND hWnd)
	: width(width), height(height), hWnd(hWnd), camera(new Camera())
{
	int size_threads = 0;

	device = nullptr;
	immContext = nullptr;
	swapChain = nullptr;
	backBuffer = nullptr;
	depthStencilView = nullptr;

	//Create the thread system here
	/*threadManager = ThreadManager::GetThreadManager();
	size_threads = threadManager->getHdwConcurrency();
	
	defContexts.reserve(size_threads);
	for (int i = 0; i < size_threads; i++)
	{
		defContexts.push_back(nullptr);
	}*/
}


GraphicsCore::~GraphicsCore()
{
	if (depthStencilView) { depthStencilView->Release(); }
	if (backBuffer) { backBuffer->Release(); }

	if (swapChain) { swapChain->Release(); }

	/*size_t numDefContexts = defContexts.size();
	for (size_t i = 0; i < numDefContexts; i++)
	{
		if (defContexts[i])
		{
			defContexts[i]->Release();
			defContexts[i] = nullptr;
		}
	}

	threadManager->Delete();*/

	if (immContext) { immContext->Release(); }
	if (device) { device->Release(); }

	depthStencilView = nullptr;
	backBuffer = nullptr;
	swapChain = nullptr;
	//threadManager = nullptr;
	immContext = nullptr;
	device = nullptr;
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

	HRESULT result = S_OK;

	//For Multipass rendering, might not be important
	//https://msdn.microsoft.com/en-us/library/windows/desktop/ff819064(v=vs.85).aspx

	result = D3D11CreateDeviceAndSwapChain(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		deviceFlags,
		0,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&swapChain,
		&device,
		&featureLvl,
		&immContext	//NULL means it won't create the immediate context
	);

	if (FAILED(result)) return result;

	//check support for multithreading
	/*D3D11_FEATURE_DATA_THREADING dxThreadSupport;
	unsigned int dxThreadSupportSize = sizeof(D3D11_FEATURE_DATA_THREADING);
	ZeroMemory(&dxThreadSupport, dxThreadSupportSize);*/

	/*result = device->CheckFeatureSupport(D3D11_FEATURE_THREADING, &dxThreadSupport, dxThreadSupportSize);
	if (FAILED(result)) return result;

	if (dxThreadSupport.DriverConcurrentCreates == FALSE || dxThreadSupport.DriverCommandLists == FALSE)
		return E_UNEXPECTED;

	int size_threads = threadManager->getHdwConcurrency();
	for (int i = 0; i < size_threads; i++)
	{
		result = device->CreateDeferredContext(0, &defContexts[i]);
		if (FAILED(result)) return result;
	}*/
	
	ID3D11Texture2D* backBufferTexture;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTexture);

	device->CreateRenderTargetView(backBufferTexture, 0, &backBuffer);
	backBufferTexture->Release();

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	ID3D11Texture2D* depthBufferTexture;
	device->CreateTexture2D(&depthStencilDesc, 0, &depthBufferTexture);
	device->CreateDepthStencilView(depthBufferTexture, 0, &depthStencilView);
	depthBufferTexture->Release();

	immContext->OMSetRenderTargets(1, &backBuffer, depthStencilView);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	immContext->RSSetViewports(1, &viewport);

	assetManager.Init(device, immContext);
	camera->updateProjection(width, height);

	DirectX::XMFLOAT4X4 identity;
	DirectX::XMStoreFloat4x4(&identity, DirectX::XMMatrixIdentity());
	matrices = {
		identity,
		camera->getViewMatrix(),
		camera->getProjectionMatrix()
	};

	return S_OK;
}

void GraphicsCore::OnResize(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;

	if (width == 0 || height == 0)
		return;

	if (depthStencilView) { depthStencilView->Release(); }
	if (backBuffer) { backBuffer->Release(); }

	swapChain->ResizeBuffers(
		1,
		width,
		height,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		0);

	ID3D11Texture2D* backBufferTexture;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBufferTexture));
	device->CreateRenderTargetView(backBufferTexture, 0, &backBuffer);
	backBufferTexture->Release();

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	ID3D11Texture2D* depthBufferTexture;
	device->CreateTexture2D(&depthStencilDesc, 0, &depthBufferTexture);
	device->CreateDepthStencilView(depthBufferTexture, 0, &depthStencilView);
	depthBufferTexture->Release();

	immContext->OMSetRenderTargets(1, &backBuffer, depthStencilView);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	immContext->RSSetViewports(1, &viewport);
}

HRESULT GraphicsCore::Init()
{
	for (auto i = objs.begin(); i != objs.end(); i++)
	{
		if (i->second != nullptr)
			i->second->Init();
	}

	return S_OK;
}

void GraphicsCore::Update()
{
	camera->update(0.01f);

	for (auto i = objs.begin(); i != objs.end(); i++)
	{
		if (i->second != nullptr)
			i->second->Update(0.01f);
	}
}

void GraphicsCore::Draw()
{
	for (auto i = objs.begin(); i != objs.end(); i++)
	{
		if (i->second != nullptr)
		{
			//calculate model matrix here
			DirectX::XMFLOAT4X4 worldMat;
			DirectX::XMVECTOR tempPosition = DirectX::XMLoadFloat3(&(i->second->transform.position));;
			DirectX::XMVECTOR tempRotation = DirectX::XMLoadFloat3(&i->second->transform.rotation);
			DirectX::XMVECTOR tempScale = DirectX::XMLoadFloat3(&i->second->transform.scale);
			DirectX::XMMATRIX modelMat;

			modelMat = DirectX::XMMatrixAffineTransformation(tempScale, DirectX::XMVectorZero(), tempRotation, tempPosition);DirectX::XMMatrixAffineTransformation(tempScale, DirectX::XMVectorZero(), tempRotation, tempPosition);

			DirectX::XMStoreFloat4x4(&worldMat, DirectX::XMMatrixTranspose(modelMat));

			matrices = {
				worldMat,
				camera->getViewMatrix(),
				camera->getProjectionMatrix()
			};

			ID3D11Buffer* cBuffer = nullptr;
			ID3D11Buffer* sbcBuffer = nullptr;

			D3D11_BUFFER_DESC cbDesc;
			cbDesc.ByteWidth = sizeof(matrices);
			cbDesc.Usage = D3D11_USAGE_DYNAMIC;
			cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cbDesc.MiscFlags = 0;
			cbDesc.StructureByteStride = 0;

			// Fill in the subresource data.
			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = &matrices;
			InitData.SysMemPitch = 0;
			InitData.SysMemSlicePitch = 0;

			// Create the buffer.
			HRESULT hr = device->CreateBuffer(&cbDesc, &InitData,
				&cBuffer);

			if (FAILED(hr))continue;

			Mesh* mesh = i->second->GetMesh();
			Material* mat = i->second->GetMaterial();
			ID3D11SamplerState* sampler = mat->getSamplerState();
			VertexShader* vshader = mat->getVShader();
			PixelShader* pshader = mat->getPShader();
			ID3D11Buffer* vertBuff = mesh->GetVertexBuffer();
			ID3D11Buffer* indexBuff = mesh->GetIndexBuffer();

			UINT stride = sizeof(Vertex);
			UINT offset = 0;

			//need to setup, send, then set Constant Buffer Data
			//Set Vertex and Pixel Shader
			//Set Vertex and Index Buffers
			//call DrawIndexed
			//immContext->UpdateSubresource(cBuffer, 0, 0, &matrices, 0, 0);
			immContext->VSSetConstantBuffers(0, 1, &cBuffer);

			if (i->second->collider->getMeshType() == MeshType::SBSphere)
			{
				sbData.collisionPoint = i->second->collider->mesh.axes[0];
				sbData.deformationVector = i->second->collider->mesh.axes[1];

				D3D11_BUFFER_DESC sbcbDesc;

				int size = sizeof(sbData);

				sbcbDesc.ByteWidth = sizeof(sbData);
				sbcbDesc.Usage = D3D11_USAGE_DYNAMIC;
				sbcbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
				sbcbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
				sbcbDesc.MiscFlags = 0;
				sbcbDesc.StructureByteStride = 0;

				// Fill in the subresource data.
				D3D11_SUBRESOURCE_DATA InitSBData;
				InitSBData.pSysMem = &sbData;
				InitSBData.SysMemPitch = 0;
				InitSBData.SysMemSlicePitch = 0;

				// Create the buffer.
				hr = device->CreateBuffer(&sbcbDesc, &InitSBData,
					&sbcBuffer);

				if (FAILED(hr))continue;

				immContext->VSSetConstantBuffers(1, 1, &sbcBuffer);
			}

			vshader->SetShader(immContext);
			pshader->SetPixelShader(immContext);
			immContext->IASetVertexBuffers(0, 1, &vertBuff,
				&stride, &offset);
			immContext->IASetIndexBuffer(indexBuff, DXGI_FORMAT_R32_UINT, 0);
			immContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);

			cBuffer->Release();
			if (sbcBuffer != nullptr) sbcBuffer->Release();
		}
	}

	//Here is where we execute the command lists
	//for (int i = 0; i < 7; i++)
	//{
	//	//Need to put in the command list
	//	immContext->ExecuteCommandList(nullptr, FALSE);
	//}

	swapChain->Present(0, 0);
}

void GraphicsCore::ClearBackAndDepthBuffers()
{
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };
	immContext->ClearRenderTargetView(backBuffer, color);
	immContext->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
}

void GraphicsCore::Quit()
{
	PostQuitMessage(0);
}
