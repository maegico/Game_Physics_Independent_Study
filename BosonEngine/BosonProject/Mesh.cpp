#include "Mesh.h"



Mesh::Mesh()
{
}

Mesh::Mesh(ID3D11Buffer * vertexBuffer, ID3D11Buffer * indexBuffer, int numIndices)
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

int Mesh::GetIndexCount()
{
	return numIndices;
}
