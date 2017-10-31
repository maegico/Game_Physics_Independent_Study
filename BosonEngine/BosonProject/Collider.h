#pragma once
#include "ColliderEnums.h"
#include "Transform.h"

class Collider
{
public:
	Collider(ColliderType colliderType, MeshType meshType, Transform& transform);
	~Collider();

	ColliderType getColliderType();
	MeshType getMeshType();

	void(*onCollision)();

private:
	ColliderType colType;
	MeshType mType;
	Transform& transform;
};

