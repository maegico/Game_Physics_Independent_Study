#include "GraphicsCore.h"

GraphicsCore::GraphicsCore()
{
}

GraphicsCore::GraphicsCore(unsigned int width, unsigned int height, HWND hWnd)
	: width(width), height(height), hWnd(hWnd)
{
	int size_threads = 0;

	device = nullptr;
	immContext = nullptr;
	swapChain = nullptr;
	backBuffer = nullptr;
	depthStencilView = nullptr;

	//Create the thread system here
	threadManager = ThreadManager::GetThreadManager();
	size_threads = threadManager->getHdwConcurrency();
	
	defContexts.reserve(size_threads);
	for (int i = 0; i < size_threads; i++)
	{
		defContexts.push_back(nullptr);
	}
}


GraphicsCore::~GraphicsCore()
{
	if (depthStencilView) { depthStencilView->Release(); }
	if (backBuffer) { backBuffer->Release(); }

	if (swapChain) { swapChain->Release(); }

	size_t numDefContexts = defContexts.size();
	for (size_t i = 0; i < numDefContexts; i++)
	{
		if (defContexts[i])
		{
			defContexts[i]->Release();
			defContexts[i] = nullptr;
		}
	}

	threadManager->Delete();

	if (immContext) { immContext->Release(); }
	if (device) { device->Release(); }

	depthStencilView = nullptr;
	backBuffer = nullptr;
	swapChain = nullptr;
	threadManager = nullptr;
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
	D3D11_FEATURE_DATA_THREADING dxThreadSupport;
	unsigned int dxThreadSupportSize = sizeof(D3D11_FEATURE_DATA_THREADING);
	ZeroMemory(&dxThreadSupport, dxThreadSupportSize);

	result = device->CheckFeatureSupport(D3D11_FEATURE_THREADING, &dxThreadSupport, dxThreadSupportSize);
	if (FAILED(result)) return result;

	if (dxThreadSupport.DriverConcurrentCreates == FALSE || dxThreadSupport.DriverCommandLists == FALSE)
		return E_UNEXPECTED;

	int size_threads = threadManager->getHdwConcurrency();
	for (int i = 0; i < size_threads; i++)
	{
		result = device->CreateDeferredContext(0, &defContexts[i]);
		if (FAILED(result)) return result;
	}
	
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

	return S_OK;
}

void GraphicsCore::OnResize(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;

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

void GraphicsCore::Draw()
{
	//Here is where we execute the command lists
	//for (int i = 0; i < 7; i++)
	//{
	//	//Need to put in the command list
	//	immContext->ExecuteCommandList(nullptr, FALSE);
	//}

	swapChain->Present(0, 0);
}

void GraphicsCore::loadMeshes(std::wstring folderPath)
{
	std::vector<std::wstring> filePaths;


	std::wstring path = folderPath + L"/*.*";

	WIN32_FIND_DATA fd;
	//for the below to work they need to run with findfilefirstW not A
	HANDLE hFind = ::FindFirstFile(path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				std::wstring temp(fd.cFileName);
				filePaths.push_back(temp);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}

	for (int i = 0; i < filePaths.size(); i++)
	{
		std::wstring name = filePaths[i];
		std::string nameTemp(name.begin(), name.end());
		Mesh mesh = loadMesh(nameTemp);
		//if (mesh == Mesh())
		//{
		//	//s
		//}
		//add meshes to mesh list
	}

}

Mesh GraphicsCore::loadMesh(std::string objFile)
{
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	std::string releasePath = "Assets/Models/";
	releasePath = releasePath + objFile;

	// File input object
	std::ifstream obj(releasePath.c_str());

	// Check for successful open
	if (!obj.is_open())
		return Mesh();

	// Variables used while reading the file
	std::vector<DirectX::XMFLOAT3> positions;     // Positions from the file
	std::vector<DirectX::XMFLOAT3> normals;       // Normals from the file
	std::vector<DirectX::XMFLOAT2> uvs;           // UVs from the file
	std::vector<Vertex> verts;           // Verts we're assembling
	std::vector<UINT> indices;           // Indices of these verts
	unsigned int vertCounter = 0;        // Count of vertices/indices
	char chars[100];                     // String for line reading

										 // Still good?
	while (obj.good())
	{
		// Get the line (100 characters should be more than enough)
		obj.getline(chars, 100);

		// Check the type of line
		if (chars[0] == 'v' && chars[1] == 'n')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			DirectX::XMFLOAT3 norm;
			sscanf_s(
				chars,
				"vn %f %f %f",
				&norm.x, &norm.y, &norm.z);

			// Add to the list of normals
			normals.push_back(norm);
		}
		else if (chars[0] == 'v' && chars[1] == 't')
		{
			// Read the 2 numbers directly into an XMFLOAT2
			DirectX::XMFLOAT2 uv;
			sscanf_s(
				chars,
				"vt %f %f",
				&uv.x, &uv.y);

			// Add to the list of uv's
			uvs.push_back(uv);
		}
		else if (chars[0] == 'v')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			DirectX::XMFLOAT3 pos;
			sscanf_s(
				chars,
				"v %f %f %f",
				&pos.x, &pos.y, &pos.z);

			// Add to the positions
			positions.push_back(pos);
		}
		else if (chars[0] == 'f')
		{
			// Read the face indices into an array
			unsigned int i[12];
			int facesRead = sscanf_s(
				chars,
				"f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
				&i[0], &i[1], &i[2],
				&i[3], &i[4], &i[5],
				&i[6], &i[7], &i[8],
				&i[9], &i[10], &i[11]);

			// - Create the verts by looking up
			//    corresponding data from vectors
			// - OBJ File indices are 1-based, so
			//    they need to be adusted
			Vertex v1;
			v1.Position = positions[i[0] - 1];
			v1.UV = uvs[i[1] - 1];
			v1.Normal = normals[i[2] - 1];

			Vertex v2;
			v2.Position = positions[i[3] - 1];
			v2.UV = uvs[i[4] - 1];
			v2.Normal = normals[i[5] - 1];

			Vertex v3;
			v3.Position = positions[i[6] - 1];
			v3.UV = uvs[i[7] - 1];
			v3.Normal = normals[i[8] - 1];

			// Flip the UV's since they're probably "upside down"
			v1.UV.y = 1.0f - v1.UV.y;
			v2.UV.y = 1.0f - v2.UV.y;
			v3.UV.y = 1.0f - v3.UV.y;

			// Add the verts to the vector
			verts.push_back(v1);
			verts.push_back(v2);
			verts.push_back(v3);

			// Add three more indices
			indices.push_back(vertCounter); vertCounter += 1;
			indices.push_back(vertCounter); vertCounter += 1;
			indices.push_back(vertCounter); vertCounter += 1;

			// Was there a 4th face?
			if (facesRead == 12)
			{
				// Make the last vertex
				Vertex v4;
				v4.Position = positions[i[9] - 1];
				v4.UV = uvs[i[10] - 1];
				v4.Normal = normals[i[11] - 1];

				// Flip the y
				v4.UV.y = 1.0f - v4.UV.y;

				// Add a whole triangle
				verts.push_back(v1);
				verts.push_back(v3);
				verts.push_back(v4);

				// Add three more indices
				indices.push_back(vertCounter); vertCounter += 1;
				indices.push_back(vertCounter); vertCounter += 1;
				indices.push_back(vertCounter); vertCounter += 1;
			}
		}
	}

	// Close the file and create the actual buffers
	obj.close();

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * (UINT)verts.size();       // 3 = number of vertices in the buffer
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells DirectX this is a vertex buffer
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// Create the proper struct to hold the initial vertex data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = &verts[0];

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	device->CreateBuffer(&vbd, &initialVertexData, &vertexBuffer);


	// Create the INDEX BUFFER description ------------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(uint32_t) * (UINT)indices.size();         // 3 = number of indices in the buffer
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is an index buffer
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Create the proper struct to hold the initial index data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = &indices[0];

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	device->CreateBuffer(&ibd, &initialIndexData, &indexBuffer);

	return Mesh(vertexBuffer, indexBuffer, indices.size());
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
