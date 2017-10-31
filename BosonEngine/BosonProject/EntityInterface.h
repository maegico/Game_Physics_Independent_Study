#pragma once
#include "Mesh.h"
#include "Transform.h"
#include "Collider.h"

class EntityInterface
{
public:
	EntityInterface(Mesh* m) :mesh(m) {}
	~EntityInterface() { delete mesh; }

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	Mesh* GetMesh() { return mesh; }

private:
	Mesh* mesh;
	Transform transform;
	//Collider collider;
	//Material* mat;
	//Can have a component pointer
	//have an add component function that adds a new component to the list (which is the pointer)
	//could create a vector for this

	//This implements the strategy pattern
	//essentially a functor
};

