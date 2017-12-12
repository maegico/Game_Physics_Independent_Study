#pragma once
#include <DirectXMath.h>

enum MeshType
{
	Sphere,
	AABB,
	OBB
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
	DirectX::XMFLOAT3 vec1;
	DirectX::XMFLOAT3 vec2;
	DirectX::XMFLOAT3 axes[3];
};