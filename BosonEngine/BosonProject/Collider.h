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

	Collider(ColliderType colliderType, MeshType meshType,
		Transform& transform, void(*onCollisionFunc)(), Vertex* vertArray);
	~Collider();

	ColliderType getColliderType();
	MeshType getMeshType();
	void setID(int id);
	int getID();

	void(*onCollision)();

	ColliderMesh getMesh();

private:
	ColliderType colType;
	MeshType mType;
	Transform& transform;
	ColliderMesh mesh;
	int id;

	bool computeColliderMesh(MeshType meshType, std::vector<Vertex>& vertArray);
};