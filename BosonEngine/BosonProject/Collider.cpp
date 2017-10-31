#include "Collider.h"

Collider::Collider(ColliderType colliderType, MeshType meshType, Transform& transform)
	: colType(colliderType), mType(meshType), transform(transform)
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
