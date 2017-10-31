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

struct ColliderMesh
{};

//Do min, max or  center, halfwidths
struct AABB : ColliderMesh
{
	DirectX::XMFLOAT3 min;
	DirectX::XMFLOAT3 max;
};

struct OBB : ColliderMesh
{
	DirectX::XMFLOAT3 min;
	DirectX::XMFLOAT3 max;
};

struct Sphere : ColliderMesh
{
	DirectX::XMFLOAT3 min;
	DirectX::XMFLOAT3 max;
};