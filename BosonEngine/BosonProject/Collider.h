#pragma once
#include <vector>
#include <DirectXMath.h>
#include "CollisionTypes.h"
#include "Transform.h"
#include "Vertex.h"

class Collider
{
public:
	bool inUse;
	Transform& transform;
	ColliderMesh mesh;

	Collider(ColliderType colliderType, ColliderMesh cmesh, Transform& transform);
	~Collider();

	ColliderType getColliderType();
	MeshType getMeshType();
	ColliderMesh getMesh();
	void setMesh(ColliderMesh cmesh);
	void setID(int id);
	int getID();
	void(*onCollision)();
private:
	ColliderType colType;
	
	int id;
};