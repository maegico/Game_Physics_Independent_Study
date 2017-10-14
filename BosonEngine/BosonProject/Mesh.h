#pragma once
#include <d3d11.h>

class Mesh
{
public:
	Mesh();
	Mesh(ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, size_t numIndices);
	~Mesh();

	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	size_t GetIndexCount();

private:
	//pointers to the Vertex Buffer and Index Buffer
	ID3D11Buffer* vertBuf;
	ID3D11Buffer* indexBuf;
	size_t numIndices;	//used when drawing
};

