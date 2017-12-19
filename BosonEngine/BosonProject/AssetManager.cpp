#include "AssetManager.h"

using namespace std;

AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
	for (auto i = m_meshes.begin(); i != m_meshes.end(); i++)
	{
		if (i->second != nullptr)
			delete i->second;
	}
	for (auto i = m_materials.begin(); i != m_materials.end(); i++)
	{
		if (i->second != nullptr)
			delete i->second;
	}
	for (auto i = m_samplers.begin(); i != m_samplers.end(); i++)
	{
		if (i->second != nullptr)
			i->second->Release();
	}
	for (auto i = m_textures.begin(); i != m_textures.end(); i++)
	{
		if (i->second != nullptr)
		{
			i->second->getTexture()->Release();
		}
	}
	for (auto i = m_vshaders.begin(); i != m_vshaders.end(); i++)
	{
		if (i->second != nullptr)
			delete i->second;
	}
	for (auto i = m_pshaders.begin(); i != m_pshaders.end(); i++)
	{
		if (i->second != nullptr)
			delete i->second;
	}
}

void AssetManager::Init(ID3D11Device * device, ID3D11DeviceContext * context)
{
	m_materials = std::unordered_map<std::string, Material*>();
	m_meshes = std::unordered_map<std::string, Mesh*>();
	m_samplers = std::unordered_map<std::string, ID3D11SamplerState*>();
	m_vshaders = std::unordered_map<std::string, VertexShader*>();
	m_pshaders = std::unordered_map<std::string, PixelShader*>();
	m_textures = std::unordered_map<std::string, Texture*>();
	//m_cubemaps = std::unordered_map<std::string, ID3D11ShaderResourceView*>();

	std::vector<std::wstring>	vshaders;
	std::vector<std::wstring>	pshaders;
	std::vector<std::wstring>	textures;
	std::vector<std::wstring>	cubemaps;
	std::vector<std::string>	models;

	FindFilesInFolderWSTR(L"Assets/VShaders", vshaders);
	FindFilesInFolderWSTR(L"Assets/PShaders", pshaders);
	FindFilesInFolderWSTR(L"Assets/Textures", textures);
	FindFilesInFolderWSTR(L"Assets/CubeMaps", cubemaps);
	FindFilesInFolder(L"Assets/Models", models);

	CreateSamplers("sampler", device);

	for (size_t i = 0; i < vshaders.size(); i++)
	{
		CreateVShader(vshaders[i], device);
	}
	for (size_t i = 0; i < pshaders.size(); i++)
	{
		CreatePShader(pshaders[i], device);
	}
	for (size_t i = 0; i < textures.size(); i++)
	{
		CreateTexture(textures[i], device, context);
	}
	for (size_t i = 0; i < cubemaps.size(); i++)
	{
		CreateCubeMap(cubemaps[i], device);
	}
	for (size_t i = 0; i < models.size(); i++)
	{
		CreateMesh(models[i], device);
	}

	LoadMaterial("base", "sampler", "VertexShader.cso", "PixelShader.cso", "");
	LoadMaterial("softbody", "sampler", "SoftBodyVShader.cso", "PixelShader.cso", "");
}

Material* AssetManager::LoadMaterial(std::string name, std::string samplerName, std::string vs, std::string ps, std::string textureName)
{
	Material* mat;
	VertexShader* vshader = m_vshaders[vs];
	PixelShader* pshader = m_pshaders[ps];
	ID3D11SamplerState*  sampler = m_samplers[samplerName];
	Texture* texture = m_textures[textureName];
	//ID3D11ShaderResourceView* normalMap = m_textures[normalMapName];
	int numTextures = 1;
	if (texture == nullptr)
		numTextures = 0;
	mat = new Material(vshader, pshader, sampler, texture, numTextures);
	m_materials[name] = mat;
	return mat;
}

Mesh* AssetManager::GetMesh(std::string mesh)
{
	return m_meshes[mesh];
}

ColliderMesh AssetManager::GetColliderMesh(std::string name)
{
	return m_colmeshes[name];
}

Material * AssetManager::GetMaterial(std::string name)
{
	return m_materials[name];
}

//took from Chris Cascioli's code
//void AssetManager::CalculateTangents(Vertex* verts, int numVerts, unsigned int* indices, int numIndices)
//{
//	// Reset tangents
//	for (int i = 0; i < numVerts; i++)
//	{
//		verts[i].Tangent = DirectX::XMFLOAT3(0, 0, 0);
//	}
//
//	// Calculate tangents one whole triangle at a time
//	for (int i = 0; i < numVerts;)
//	{
//		// Grab indices and vertices of first triangle
//		unsigned int i1 = indices[i++];
//		unsigned int i2 = indices[i++];
//		unsigned int i3 = indices[i++];
//		Vertex* v1 = &verts[i1];
//		Vertex* v2 = &verts[i2];
//		Vertex* v3 = &verts[i3];
//
//		// Calculate vectors relative to triangle positions
//		float x1 = v2->Position.x - v1->Position.x;
//		float y1 = v2->Position.y - v1->Position.y;
//		float z1 = v2->Position.z - v1->Position.z;
//
//		float x2 = v3->Position.x - v1->Position.x;
//		float y2 = v3->Position.y - v1->Position.y;
//		float z2 = v3->Position.z - v1->Position.z;
//
//		// Do the same for vectors relative to triangle uv's
//		float s1 = v2->UV.x - v1->UV.x;
//		float t1 = v2->UV.y - v1->UV.y;
//
//		float s2 = v3->UV.x - v1->UV.x;
//		float t2 = v3->UV.y - v1->UV.y;
//
//		// Create vectors for tangent calculation
//		float r = 1.0f / (s1 * t2 - s2 * t1);
//
//		float tx = (t2 * x1 - t1 * x2) * r;
//		float ty = (t2 * y1 - t1 * y2) * r;
//		float tz = (t2 * z1 - t1 * z2) * r;
//
//		// Adjust tangents of each vert of the triangle
//		v1->Tangent.x += tx;
//		v1->Tangent.y += ty;
//		v1->Tangent.z += tz;
//
//		v2->Tangent.x += tx;
//		v2->Tangent.y += ty;
//		v2->Tangent.z += tz;
//
//		v3->Tangent.x += tx;
//		v3->Tangent.y += ty;
//		v3->Tangent.z += tz;
//	}
//
//	// Ensure all of the tangents are orthogonal to the normals
//	for (int i = 0; i < numVerts; i++)
//	{
//		// Grab the two vectors
//		DirectX::XMVECTOR normal = XMLoadFloat3(&verts[i].Normal);
//		DirectX::XMVECTOR tangent = XMLoadFloat3(&verts[i].Tangent);
//
//		//DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(tangent1, DirectX::XMVectorMultiply(normal1, DirectX::XMVector3Dot(normal1, tangent1))));
//
//		// Use Gram-Schmidt orthogonalize
//		tangent = DirectX::XMVector3Normalize(
//			DirectX::XMVectorSubtract(tangent, DirectX::XMVectorMultiply(normal, DirectX::XMVector3Dot(normal, tangent))));
//
//		// Store the tangent
//		DirectX::XMStoreFloat3(&verts[i].Tangent, tangent);
//	}
//}

void AssetManager::CreateMesh(std::string objFile, ID3D11Device* device)
{
	MeshType mType = MeshType::AABB;
	std::string releasePath = "Assets/Models/";
	releasePath = releasePath + objFile;

	// File input object
	std::ifstream obj(releasePath.c_str());

	// Check for successful open
	if (!obj.is_open())
		return;

	// Variables used while reading the file
	std::vector<DirectX::XMFLOAT3> positions;     // Positions from the file
	std::vector<DirectX::XMFLOAT3> normals;       // Normals from the file
	//std::vector<DirectX::XMFLOAT3> tangents;       // Tangents calculated
	//std::vector<DirectX::XMFLOAT3> binormals;       // Binormals calculated
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
		else if (chars[0] == 'm')
		{
			//int num = 1;
			sscanf_s(chars, "m %d", &mType);
			//mType = (MeshType)num;
		}
	}

	// Close the file and create the actual buffers
	obj.close();

	//CalculateTangents(&verts[0], verts.size(), &indices[0], indices.size());
	
	ColliderMesh cmesh = ColliderMeshFunctions::computeColliderMesh(mType, verts);
	m_colmeshes[objFile] = cmesh;

	//right here we need to create the vertex buffer and the index buffer

	ID3D11Buffer* vertBuf;
	ID3D11Buffer* indexBuf;

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = (UINT)(sizeof(Vertex) * verts.size());       // 3 = number of vertices in the buffer
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
	device->CreateBuffer(&vbd, &initialVertexData, &vertBuf);


	// Create the INDEX BUFFER description ------------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = (UINT)(sizeof(uint32_t) * indices.size());
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
	device->CreateBuffer(&ibd, &initialIndexData, &indexBuf);

	m_meshes[objFile] = new Mesh(vertBuf, indexBuf, indices.size());
	//m_meshes[objFile] = new Mesh(&verts[0], &indices[0], verts.size(), indices.size(), device);
}

void AssetManager::CreateSamplers(std::string name, ID3D11Device* device)
{
	ID3D11SamplerState*  sampler;

	D3D11_SAMPLER_DESC samplerDes;
	memset(&samplerDes, 0, sizeof(D3D11_SAMPLER_DESC));
	//Address U, V, W
	samplerDes.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDes.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDes.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//Filter
	samplerDes.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	//trilinear filtering
															//MaxLOD
	samplerDes.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT result = device->CreateSamplerState(&samplerDes, &sampler);
	if (result != S_OK)
		printf("ERROR: Failed to create Sampler State.");
	else
		m_samplers[name] = sampler;
}

void AssetManager::CreateTexture(std::wstring textureName, ID3D11Device* device, ID3D11DeviceContext* context)
{
	std::wstring releasePath = L"Debug/Assets/Textures/";
	releasePath = releasePath + textureName;
	std::wstring debugPath = releasePath.substr(6, releasePath.length() - 6);

	ID3D11ShaderResourceView* texture;

	HRESULT result = DirectX::CreateWICTextureFromFile(device, context, debugPath.c_str(), 0, &texture);
	if (result != S_OK)
		printf("ERROR: Failed to Load Texture.");

	std::string name(textureName.begin(), textureName.end());
	m_textures[name] = new Texture(texture, Texture2D);
}

void AssetManager::CreateCubeMap(std::wstring cubeName, ID3D11Device* device)
{
	std::wstring debugPath = L"Assets/CubeMaps/";
	debugPath += cubeName;

	ID3D11ShaderResourceView* cubemap;
	DirectX::CreateDDSTextureFromFile(device, debugPath.c_str(), 0, &cubemap);

	std::string name(cubeName.begin(), cubeName.end());
	m_textures[name] = new Texture(cubemap, TextureType::CubeMap);
}

//Compiles .cso's where the .hlsl file is
void AssetManager::CreateVShader(std::wstring shader, ID3D11Device* device)
{
	std::wstring compiledName = shader.substr(0, shader.length() - 4);
	compiledName += L"cso";
	std::wstring releasePath = L"../x64/Debug/";
	releasePath = releasePath + compiledName;

	std::wstring debugPath = L"Assets/VShaders/" + compiledName;

	ID3DBlob* shaderBlob;
	ID3D11VertexShader* vshader;
	ID3D11InputLayout* layout;

	HRESULT hr = D3DReadFileToBlob(releasePath.c_str(), &shaderBlob);
	if (hr != S_OK)
		hr = D3DReadFileToBlob(debugPath.c_str(), &shaderBlob);
	if (hr != S_OK)
	{
		Debug::Log("Failed to open/read Vertex Shader file", LogType::Error);
		return;
	}

	hr = device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &vshader);
	if (hr != S_OK)
	{
		Debug::Log("Failed to create Vertex Shader", LogType::Error);
		return;
	}
	//context->VSSetShader(vshader, 0, 0);

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTCOORD",0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	hr = device->CreateInputLayout(&ied[0], 3, shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &layout);
	if (hr != S_OK)
	{
		Debug::Log("Failed to create input layout", LogType::Error);
		return;
	}

	//context->IASetInputLayout(layout);
	shaderBlob->Release();
	std::string name(compiledName.begin(), compiledName.end());
	m_vshaders[name] = new VertexShader(vshader, layout);
}

void AssetManager::CreatePShader(std::wstring shader, ID3D11Device* device)
{
	std::wstring compiledName = shader.substr(0, shader.length() - 4);
	compiledName += L"cso";
	std::wstring releasePath = L"../x64/Debug/";
	releasePath = releasePath + compiledName;

	std::wstring debugPath = L"Assets/VShaders/" + compiledName;

	ID3DBlob* shaderBlob;
	ID3D11PixelShader* pshader;

	HRESULT hr = D3DReadFileToBlob(releasePath.c_str(), &shaderBlob);
	if (hr != S_OK)
		hr = D3DReadFileToBlob(debugPath.c_str(), &shaderBlob);
	if (hr != S_OK)
	{
		Debug::Log("Failed to open/read Pixel Shader file", LogType::Error);
		return;
	}

	hr = device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &pshader);
	if (hr != S_OK)
	{
		Debug::Log("Failed to create Pixel Shader file", LogType::Error);
		return;
	}

	//context->PSSetShader(pshader, 0, 0);
	shaderBlob->Release();
	std::string shaderString(compiledName.begin(), compiledName.end());
	m_pshaders[shaderString] = new PixelShader(pshader);
}

//I could make the below better, by

//I took the basic code from below and modified it to work for both UNICODE and non-UNICODE character sets
//Got it from: http://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
void AssetManager::FindFilesInFolder(std::wstring folder, std::vector<std::string>& listOfFiles)
{
#ifdef UNICODE
	std::wstring path = folder + L"/*.*";
	
	WIN32_FIND_DATA fd;
	//for the below to work they need to run with findfilefirstW not A
	HANDLE hFind = ::FindFirstFile(path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				std::wstring temp1(fd.cFileName);
				std::string temp2(temp1.begin(), temp1.end());
				listOfFiles.push_back(temp2);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}

#else
	
	std::wstring path = folder + L"/*.*";
	std::string pathSTR(path.begin(), path.end());
	WIN32_FIND_DATA fd;
	//for the below to work they need to run with findfilefirstW not A
	HANDLE hFind = ::FindFirstFile(pathSTR.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {

				std::string temp(fd.cFileName);
				listOfFiles.push_back(temp);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
#endif
}

//I didn't code this
//Got it from: http://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
void AssetManager::FindFilesInFolderWSTR(std::wstring folder, std::vector<std::wstring>& listOfFiles)
{
#ifdef UNICODE
	std::wstring path = folder + L"/*.*";

	WIN32_FIND_DATA fd;
	//for the below to work they need to run with findfilefirstW not A
	HANDLE hFind = ::FindFirstFile(path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				std::wstring temp(fd.cFileName);
				listOfFiles.push_back(temp);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}

#else

	std::wstring path = folder + L"/*.*";
	std::string pathSTR(path.begin(), path.end());
	WIN32_FIND_DATA fd;
	//for the below to work they need to run with findfilefirstW not A
	HANDLE hFind = ::FindFirstFile(pathSTR.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {

				std::string temp1(fd.cFileName);
				std::wstring temp2(temp1.begin(), temp1.end());
				listOfFiles.push_back(temp2);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
#endif
}
