#include "BasicEntity.h"



BasicEntity::BasicEntity(Mesh* mesh, ColliderMesh* cmesh, Material* mat)
	: EntityInterface(mesh, mat, ColliderType::Rigidbody, cmesh)
{
}


BasicEntity::~BasicEntity()
{
}

void BasicEntity::Init()
{
}

void BasicEntity::Update()
{
}

void BasicEntity::Draw()
{
}
