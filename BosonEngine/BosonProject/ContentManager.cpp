#include "ContentManager.h"
#include <cstdarg>

using namespace std;

ContentManager::ContentManager()
{
}

ContentManager::~ContentManager()
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
			i->second->Release();
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
	
	m_context->Release();
	m_device->Release();
}

void ContentManager::Init(ID3D11Device * device, ID3D11DeviceContext * context)
{
	device->AddRef();
	context->AddRef();

	m_device = device;
	m_context = context;

	m_materials = std::unordered_map<std::string, Material*>();
	m_meshes = std::unordered_map<std::string, Mesh*>();
	m_samplers = std::unordered_map<std::string, ID3D11SamplerState*>();
	m_vshaders = std::unordered_map<std::string, SimpleVertexShader*>();
	m_pshaders = std::unordered_map<std::string, SimplePixelShader*>();
	m_textures = std::unordered_map<std::string, ID3D11ShaderResourceView*>();
	m_cubemaps = std::unordered_map<std::string, ID3D11ShaderResourceView*>();
	

	//below is placed on the stack, since I won't need them after this
	//below here is probably where I will probe the files for the needed info
	//the below isn't so terrible anymore, but still
	//these make sense though since we will go through all the files in a file location and grab all there names saving them inside the below vectors and more vectors
	std::vector<std::wstring>	vshaders;	//see if I can change this <- this is going to be terrible
	std::vector<std::wstring>	pshaders;	//see if I can change this <- this is going to be terrible
	std::vector<std::wstring>	textures;
	std::vector<std::wstring>	cubemaps;
	std::vector<std::string>	models;

	FindFilesInFolderWSTR(L"Assets/VShaders", vshaders);
	FindFilesInFolderWSTR(L"Assets/PShaders", pshaders);
	FindFilesInFolderWSTR(L"Assets/Textures", textures);
	FindFilesInFolderWSTR(L"Assets/CubeMaps", cubemaps);
	FindFilesInFolder(L"Assets/Models", models);

	CreateSamplers("sampler");

	//The below isn't creating the shaders correctly
	for (int i = 0; i < vshaders.size(); i++)
	{
		CreateVShader(vshaders[i]);
	}
	for (int i = 0; i < pshaders.size(); i++)
	{
		CreatePShader(pshaders[i]);
	}
	for (int i = 0; i < textures.size(); i++)
	{
		CreateTexture(textures[i]);
	}
	for (int i = 0; i < cubemaps.size(); i++)
	{
		CreateCubeMap(cubemaps[i]);
	}
	for (int i = 0; i < models.size(); i++)
	{
		CreateMesh(models[i]);
	}
}

//Should I hold a bunch of textures in CM or create on construction of a material
Material* ContentManager::LoadMaterialWithNormalMap(std::string name, std::string samplerName, std::string vs, std::string ps, std::string textureName, std::string normalMapName)
{
	Material* mat;
	SimpleVertexShader* vshader = m_vshaders[vs];
	SimplePixelShader* pshader = m_pshaders[ps];
	ID3D11SamplerState*  sampler = m_samplers[samplerName];
	ID3D11ShaderResourceView* texture = m_textures[textureName];
	ID3D11ShaderResourceView* normalMap = m_textures[normalMapName];

	mat = new Material(vshader, pshader, texture, normalMap, sampler);
	m_materials[name] = mat;
	return mat;
}

Material * ContentManager::LoadCubeMapMaterial(std::string name, std::string samplerName, std::string vs, std::string ps, std::string textureName)
{
	Material* mat;
	SimpleVertexShader* vshader = m_vshaders[vs];
	SimplePixelShader* pshader = m_pshaders[ps];
	ID3D11SamplerState*  sampler = m_samplers[samplerName];
	ID3D11ShaderResourceView* texture = m_cubemaps[textureName];

	mat = new Material(vshader, pshader, texture, nullptr, sampler);
	m_materials[name] = mat;

	return mat;
}

Material * ContentManager::LoadPostProcessingMaterial(std::string name, std::string samplerName, std::string vs, std::string ps)
{
	Material* mat;
	SimpleVertexShader* vshader = m_vshaders[vs];
	SimplePixelShader* pshader = m_pshaders[ps];
	ID3D11SamplerState*  sampler = m_samplers[samplerName];

	mat = new Material(vshader, pshader, nullptr, nullptr, sampler);
	m_materials[name] = mat;
	return mat;
}

Mesh* ContentManager::GetMesh(std::string mesh)
{
	return m_meshes[mesh];
}

Material * ContentManager::GetMaterial(std::string name)
{
	return m_materials[name];
}

//took from Chris Cascioli's code
void ContentManager::CalculateTangents(Vertex* verts, int numVerts, unsigned int* indices, int numIndices)
{
	// Reset tangents
	for (int i = 0; i < numVerts; i++)
	{
		verts[i].Tangent = DirectX::XMFLOAT3(0, 0, 0);
	}

	// Calculate tangents one whole triangle at a time
	for (int i = 0; i < numVerts;)
	{
		// Grab indices and vertices of first triangle
		unsigned int i1 = indices[i++];
		unsigned int i2 = indices[i++];
		unsigned int i3 = indices[i++];
		Vertex* v1 = &verts[i1];
		Vertex* v2 = &verts[i2];
		Vertex* v3 = &verts[i3];

		// Calculate vectors relative to triangle positions
		float x1 = v2->Position.x - v1->Position.x;
		float y1 = v2->Position.y - v1->Position.y;
		float z1 = v2->Position.z - v1->Position.z;

		float x2 = v3->Position.x - v1->Position.x;
		float y2 = v3->Position.y - v1->Position.y;
		float z2 = v3->Position.z - v1->Position.z;

		// Do the same for vectors relative to triangle uv's
		float s1 = v2->UV.x - v1->UV.x;
		float t1 = v2->UV.y - v1->UV.y;

		float s2 = v3->UV.x - v1->UV.x;
		float t2 = v3->UV.y - v1->UV.y;

		// Create vectors for tangent calculation
		float r = 1.0f / (s1 * t2 - s2 * t1);

		float tx = (t2 * x1 - t1 * x2) * r;
		float ty = (t2 * y1 - t1 * y2) * r;
		float tz = (t2 * z1 - t1 * z2) * r;

		// Adjust tangents of each vert of the triangle
		v1->Tangent.x += tx;
		v1->Tangent.y += ty;
		v1->Tangent.z += tz;

		v2->Tangent.x += tx;
		v2->Tangent.y += ty;
		v2->Tangent.z += tz;

		v3->Tangent.x += tx;
		v3->Tangent.y += ty;
		v3->Tangent.z += tz;
	}

	// Ensure all of the tangents are orthogonal to the normals
	for (int i = 0; i < numVerts; i++)
	{
		// Grab the two vectors
		DirectX::XMVECTOR normal = XMLoadFloat3(&verts[i].Normal);
		DirectX::XMVECTOR tangent = XMLoadFloat3(&verts[i].Tangent);

		//DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(tangent1, DirectX::XMVectorMultiply(normal1, DirectX::XMVector3Dot(normal1, tangent1))));

		// Use Gram-Schmidt orthogonalize
		tangent = DirectX::XMVector3Normalize(
			DirectX::XMVectorSubtract(tangent, DirectX::XMVectorMultiply(normal, DirectX::XMVector3Dot(normal, tangent))));

		// Store the tangent
		DirectX::XMStoreFloat3(&verts[i].Tangent, tangent);
	}
}

void ContentManager::CreateMesh(std::string objFile)
{
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
	std::vector<DirectX::XMFLOAT3> tangents;       // Tangents calculated
	std::vector<DirectX::XMFLOAT3> binormals;       // Binormals calculated
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

	CalculateTangents(&verts[0], verts.size(), &indices[0], indices.size());
	
	m_meshes[objFile] = new Mesh(&verts[0], &indices[0], verts.size(), indices.size(), m_device);
}

void ContentManager::CreateSamplers(std::string name)
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

	HRESULT result = m_device->CreateSamplerState(&samplerDes, &sampler);
	if (result != S_OK)
		printf("ERROR: Failed to create Sampler State.");
	else
		m_samplers[name] = sampler;
}

void ContentManager::CreateTexture(std::wstring textureName)
{
	std::wstring releasePath = L"Debug/Assets/Textures/";
	releasePath = releasePath + textureName;
	std::wstring debugPath = releasePath.substr(6, releasePath.length() - 6);

	ID3D11ShaderResourceView* texture;

	HRESULT result = DirectX::CreateWICTextureFromFile(m_device, m_context, debugPath.c_str(), 0, &texture);
	if (result != S_OK)
		printf("ERROR: Failed to Load Texture.");
	std::string name(textureName.begin(), textureName.end());
	m_textures[name] = texture;
}

void ContentManager::CreateCubeMap(std::wstring cubeName)
{
	std::wstring debugPath = L"Assets/CubeMaps/";
	debugPath += cubeName;

	ID3D11ShaderResourceView* cubemap;

	DirectX::CreateDDSTextureFromFile(m_device, debugPath.c_str(), 0, &cubemap);

	std::string name(cubeName.begin(), cubeName.end());
	m_cubemaps[name] = cubemap;
}

//Compiles .cso's where the .hlsl file is
void ContentManager::CreateVShader(std::wstring shader)
{
	std::wstring compiledName = shader.substr(0, shader.length() - 4);
	compiledName += L"cso";
	std::wstring releasePath = L"Debug/";
	releasePath = releasePath + compiledName;

	std::wstring debugPath = L"Assets/VShaders/" + compiledName;
	//std::wstring debugPath = compiledName;

	//wchar_t begin[] = L"Debug/\0";
	//size_t len = wcslen(shader);
	//wchar_t* projDirFilePath = wcsncat(begin, shader, len+1);
	//Want to figure the above out!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! <- !!!!

	SimpleVertexShader* vertexShader = new SimpleVertexShader(m_device, m_context);
	if (!vertexShader->LoadShaderFile(releasePath.c_str()))
		vertexShader->LoadShaderFile(debugPath.c_str());

	// You'll notice that the code above attempts to load each
	// compiled shader file (.cso) from two different relative paths.

	// This is because the "working directory" (where relative paths begin)
	// will be different during the following two scenarios:
	//  - Debugging in VS: The "Project Directory" (where your .cpp files are) 
	//  - Run .exe directly: The "Output Directory" (where the .exe & .cso files are)

	// Checking both paths is the easiest way to ensure both 
	// scenarios work correctly, although others exist

	std::string name(compiledName.begin(), compiledName.end());
	m_vshaders[name] = vertexShader;
}

void ContentManager::CreatePShader(std::wstring shader)
{
	std::wstring compiledName = shader.substr(0, shader.length() - 4);
	compiledName += L"cso";
	std::wstring releasePath = L"Debug/";
	releasePath = releasePath + compiledName;

	std::wstring debugPath = L"Assets/VShaders/" + compiledName;

	SimplePixelShader* pixelShader = new SimplePixelShader(m_device, m_context);
	if (!pixelShader->LoadShaderFile(releasePath.c_str()))
		pixelShader->LoadShaderFile(compiledName.c_str());

	// You'll notice that the code above attempts to load each
	// compiled shader file (.cso) from two different relative paths.

	// This is because the "working directory" (where relative paths begin)
	// will be different during the following two scenarios:
	//  - Debugging in VS: The "Project Directory" (where your .cpp files are) 
	//  - Run .exe directly: The "Output Directory" (where the .exe & .cso files are)

	// Checking both paths is the easiest way to ensure both 
	// scenarios work correctly, although others exist

	std::string shaderString(compiledName.begin(), compiledName.end());
	m_pshaders[shaderString] = pixelShader;
}

//I could make the below better, by

//I took the basic code from below and modified it to work for both UNICODE and non-UNICODE character sets
//Got it from: http://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
void ContentManager::FindFilesInFolder(std::wstring folder, std::vector<std::string>& listOfFiles)
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
void ContentManager::FindFilesInFolderWSTR(std::wstring folder, std::vector<std::wstring>& listOfFiles)
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
