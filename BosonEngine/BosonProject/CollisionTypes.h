#pragma once
#include <DirectXMath.h>
#include "Vertex.h"
#include <vector>

enum MeshType
{
	Sphere,
	AABB,
	OBB,
	SBSphere	//Stands for Soft Body Sphere
};

enum ColliderType
{
	Static,
	Rigidbody
};

//for AABB, vec1 = min & vec2 = max
//for OBB, vec1 = center, vec2 = halfwidths, axes = local axes
//for Sphere, vec1 = center, vec2 =  (radius, nothing, nothing)
class ColliderMesh
{
public:
	MeshType mType;
	DirectX::XMFLOAT3 vec1;
	DirectX::XMFLOAT3 vec2;
	DirectX::XMFLOAT3 axes[3];
};

namespace ColliderMeshFunctions
{

	using namespace DirectX;

	DirectX::XMFLOAT3 halfpoint(DirectX::XMFLOAT3 vec1, DirectX::XMFLOAT3 vec2);
	ColliderMesh computeColliderMesh(MeshType meshType, std::vector<Vertex>& vertArray);
}