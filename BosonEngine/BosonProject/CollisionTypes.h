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

//for AABB & OBB, vec1 = min & vec2 = max
//for Sphere, vec1 = center, vec2 =  (radius, nothing, nothing)
class ColliderMesh
{
public:
	DirectX::XMFLOAT3 vec1;
	DirectX::XMFLOAT3 vec2;
};

//Do min, max or  center, halfwidths
//class AABBMesh : ColliderMesh
//{
//public:
//	DirectX::XMFLOAT3 min;
//	DirectX::XMFLOAT3 max;
//};
//
//class OBBMesh : ColliderMesh
//{
//public:
//	DirectX::XMFLOAT3 min;
//	DirectX::XMFLOAT3 max;
//};
//
//class SphereMesh : ColliderMesh
//{
//public:
//	DirectX::XMFLOAT3 center;
//	float radius;
//};