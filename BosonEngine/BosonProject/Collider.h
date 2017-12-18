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
	ColliderMesh mesh;
	int id;
};