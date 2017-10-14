#include "Mesh.h"



Mesh::Mesh()
{
}

Mesh::Mesh(ID3D11Buffer * vertexBuffer, ID3D11Buffer * indexBuffer, size_t numIndices)
	: vertBuf(vertexBuffer), indexBuf(indexBuffer), numIndices(numIndices)
{
}


Mesh::~Mesh()
{
}

ID3D11Buffer * Mesh::GetVertexBuffer()
{
	return vertBuf;
}

ID3D11Buffer * Mesh::GetIndexBuffer()
{
	return indexBuf;
}

size_t Mesh::GetIndexCount()
{
	return numIndices;
}
