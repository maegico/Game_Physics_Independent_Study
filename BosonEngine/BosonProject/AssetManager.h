#pragma once
#include <unordered_map>
#include <vector>
#include <fstream>
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

#include "CollisionTypes.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture.h"
#include "Vertex.h"
#include "Material.h"
#include "Mesh.h"
#include "Debug.h"

#pragma comment(lib, "d3dcompiler.lib")

class AssetManager
{
	//handle model, texture, and shader loading
	//http://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
		//used second solution
public:
	//default constructor (Don't Use)
	AssetManager();
	~AssetManager();

	void Init(ID3D11Device* device, ID3D11DeviceContext* context);

	Material* LoadMaterial(std::string name, std::string samplerName, std::string vs, std::string ps, std::string textureName);
	/*Material* LoadMaterialWithNormalMap(std::string name, std::string samplerName, std::string vs, std::string ps, std::string textureName, std::string normalMapName);
	Material* LoadCubeMapMaterial(std::string name, std::string samplerName, std::string vs, std::string ps, std::string textureName);
	Material* LoadPostProcessingMaterial(std::string name, std::string samplerName, std::string vs, std::string ps);*/
	Mesh* GetMesh(std::string);
	ColliderMesh* GetColliderMesh(std::string);
	Material* GetMaterial(std::string);

private:
	std::unordered_map<std::string, Mesh*> m_meshes;	//List of meshes
	std::unordered_map<std::string, ID3D11SamplerState*> m_samplers;	//List of sampler states
	std::unordered_map<std::string, Texture*> m_textures;	//List of textures
	//std::unordered_map<std::string, ID3D11ShaderResourceView*>	m_cubemaps;	//List of textures
	std::unordered_map<std::string, VertexShader*>	m_vshaders;	//List of vertex shaders
	std::unordered_map<std::string, PixelShader*> m_pshaders;	//List of pixel shaders
	std::unordered_map<std::string, Material*>	m_materials;//List of materials
	std::unordered_map<std::string, ColliderMesh*> m_colmeshes;

	//Calculate the Tangents
	//void CalculateTangents(Vertex* verts, int numVerts, unsigned int* indices, int numIndices);
	//Creates a mesh of the passed in .obj file and save it into a std::map  
	void CreateMesh(std::string objFile, ID3D11Device* device);
	//Creates a sampler states and save it into a std::map using the passed in name
	void CreateSamplers(std::string name, ID3D11Device* m_device);
	void CreateTexture(std::wstring textureName, ID3D11Device* device, ID3D11DeviceContext* context);
	void CreateCubeMap(std::wstring cubeName, ID3D11Device* device);
	//Creates a vertex shader of the passed in wide string .cso file and saves it into a std::map
	void CreateVShader(std::wstring shader, ID3D11Device* device);
	//Creates a pixel shader of the passed in wide string .cso file and saves it into a std::map
	void CreatePShader(std::wstring shader, ID3D11Device* device);

	void FindFilesInFolder(std::wstring folder, std::vector<std::string>& listOfFiles);
	void FindFilesInFolderWSTR(std::wstring folder, std::vector<std::wstring>& listOfFiles);
};

