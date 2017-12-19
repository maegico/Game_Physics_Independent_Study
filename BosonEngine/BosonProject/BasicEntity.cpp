#include "BasicEntity.h"

BasicEntity::BasicEntity(Mesh* mesh, ColliderMesh cmesh, Material* mat)
	: EntityInterface(mesh, mat, ColliderType::Rigidbody, cmesh)
{
}

BasicEntity::~BasicEntity()
{
}

void collisionResolution()
{
	printf("Colliding!!!!\n");
}

void BasicEntity::Init()
{
	collider->onCollision = collisionResolution;
}

void BasicEntity::Update(float deltaTime)
{
	EntityInterface::Update(deltaTime);
}

void BasicEntity::Draw()
{
}
