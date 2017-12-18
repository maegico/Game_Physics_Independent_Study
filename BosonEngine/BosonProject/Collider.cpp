#include "Collider.h"

using namespace DirectX;

Collider::Collider(ColliderType colliderType, ColliderMesh cmesh, Transform& transform)
	: inUse(true), id(-1), colType(colliderType), transform(transform), mesh(cmesh), onCollision(nullptr)
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
	return mesh.mType;
}

void Collider::setID(int id)
{
	this->id = id;
}

int Collider::getID()
{
	return id;
}

ColliderMesh Collider::getMesh()
{
	return mesh;
}

void Collider::setMesh(ColliderMesh cmesh)
{
	mesh = cmesh;
}
