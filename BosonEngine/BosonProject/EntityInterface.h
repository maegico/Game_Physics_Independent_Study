#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "Collider.h"

class EntityInterface
{
public:
	EntityInterface(Mesh* mesh, Material* mat)
		: mesh(mesh), mat(mat)
	{
		transform = { DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(0,0,0) };
	}
	~EntityInterface() { delete mesh; }

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	Mesh* GetMesh() { return mesh; }
	Material* GetMaterial() { return mat; }

private:
	Mesh* mesh;
	Material* mat;
	Transform transform;
	//Collider collider;
	//Material* mat;
	//Can have a component pointer
	//have an add component function that adds a new component to the list (which is the pointer)
	//could create a vector for this

	//This implements the strategy pattern
	//essentially a functor
};

