#include "Collider.h"

Collider::Collider(ColliderType colliderType, MeshType meshType,
	Transform& transform, void(*onCollisionFunc)(), Vertex* vertArray)
	: inUse(false), colType(colliderType), mType(meshType), transform(transform),
	  onCollision(onCollisionFunc)
{
}


Collider::~Collider()
{
}

ColliderType Collider::getColliderType()
{
	return colType;
}

MeshType Collider::getMeshType()
{
	return mType;
}

void Collider::setID(int id)
{
	this->id = id;
}

int Collider::getID()
{
	return id;
}

bool Collider::computeColliderMesh(MeshType meshType, Vertex* vertArray)
{
	switch (meshType)
	{
	case MeshType::AABB:
		break;
	case MeshType::OBB:
		break;
	case MeshType::Sphere:
		break;
	default:
		break;
	}
	return false;
}
