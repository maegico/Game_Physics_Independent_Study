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

	Collider(ColliderType colliderType, ColliderMesh* cmesh, Transform& transform);
	~Collider();

	ColliderType getColliderType();
	MeshType getMeshType();
	ColliderMesh* getMesh();
	void setID(int id);
	int getID();
	void(*onCollision)();
private:
	ColliderType colType;
	Transform& transform;
	ColliderMesh* mesh;
	int id;
};