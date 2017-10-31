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

class ColliderMesh
{};

//Do min, max or  center, halfwidths
class AABBMesh : ColliderMesh
{
public:
	DirectX::XMFLOAT3 min;
	DirectX::XMFLOAT3 max;
};

class OBBMesh : ColliderMesh
{
public:
	DirectX::XMFLOAT3 min;
	DirectX::XMFLOAT3 max;
};

class SphereMesh : ColliderMesh
{
public:
	DirectX::XMFLOAT3 center;
	float radius;
};