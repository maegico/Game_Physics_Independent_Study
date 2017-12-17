#include "Collider.h"

using namespace DirectX;

Collider::Collider(ColliderType colliderType, ColliderMesh* cmesh, Transform& transform)
	: inUse(false), colType(colliderType), transform(transform), mesh(cmesh), onCollision(nullptr)
{
	//this->transform = transform;
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
	return mesh->mType;
}

void Collider::setID(int id)
{
	this->id = id;
}

int Collider::getID()
{
	return id;
}

ColliderMesh* Collider::getMesh()
{
	return mesh;
}
