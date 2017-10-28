#pragma once
//#include <cstdint>
#include <vector>
#include <fstream>
#include <map>
#include <memory>
#include <Windows.h>
#include <d3d11.h>

#include <WICTextureLoader.h>
#include "DDSTextureLoader.h"

#include "VertexShader.h"
#include "PixelShader.h"
//#include "SimpleShader.h"
#include "Vertex.h"
#include "Material.h"
#include "Mesh.h"

class ContentManager
{
	//handle model, texture, and shader loading
	//http://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
		//use second solution
public:
	//default constructor (Don't Use)
	ContentManager();
	~ContentManager();

	void Init(ID3D11Device* device, ID3D11DeviceContext* context);

	Material* LoadMaterialWithNormalMap(std::string name, std::string samplerName, std::string vs, std::string ps, std::string textureName, std::string normalMapName);
	Material* LoadCubeMapMaterial(std::string name, std::string samplerName, std::string vs, std::string ps, std::string textureName);
	Material* LoadPostProcessingMaterial(std::string name, std::string samplerName, std::string vs, std::string ps);
	Mesh* GetMesh(std::string);
	Material* GetMaterial(std::string);


	//Unordered Map implementation was breaking, will work on it some more later\
		//did not seem to be the issue
private:
	//could possibly use an unordered map
	//also, need to allow for multiple textures
	std::unordered_map<std::string, Mesh*>						m_meshes;	//List of meshes
	std::unordered_map<std::string, ID3D11SamplerState*>		m_samplers;	//List of sampler states
	std::unordered_map<std::string, ID3D11ShaderResourceView*>	m_textures;	//List of textures
	std::unordered_map<std::string, ID3D11ShaderResourceView*>	m_cubemaps;	//List of textures
	std::unordered_map<std::string, SimpleVertexShader*>		m_vshaders;	//List of vertex shaders
	std::unordered_map<std::string, SimplePixelShader*>			m_pshaders;	//List of pixel shaders
	std::unordered_map<std::string, Material*>					m_materials;//List of materials

	ID3D11Device*			m_device;		//Pointer to the D3D11 Device
	ID3D11DeviceContext*	m_context;		//Pointer to the D3D11 Device Context

	//Calculate the Tangents
	void CalculateTangents(Vertex* verts, int numVerts, unsigned int* indices, int numIndices);
	//Creates a mesh of the passed in .obj file and save it into a std::map  
	void CreateMesh(std::string objFile);
	//Creates a sampler states and save it into a std::map using the passed in name
	void CreateSamplers(std::string name);
	void CreateTexture(std::wstring textureName);
	void CreateCubeMap(std::wstring cubeName);
	//Creates a vertex shader of the passed in wide string .cso file and saves it into a std::map
	void CreateVShader(std::wstring shader);
	//Creates a pixel shader of the passed in wide string .cso file and saves it into a std::map
	void CreatePShader(std::wstring shader);

	void FindFilesInFolder(std::wstring folder, std::vector<std::string>& listOfFiles);
	void FindFilesInFolderWSTR(std::wstring folder, std::vector<std::wstring>& listOfFiles);
};

