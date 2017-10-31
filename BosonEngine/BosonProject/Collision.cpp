#include "Collision.h"

Collision::CollisionSystem::CollisionSystem()
{
}

Collision::CollisionSystem::~CollisionSystem()
{
}

void Collision::CollisionSystem::BatchCollider(Collider& collider)
{
	if (collider.getID() == 0) return;

	collider.setID(++idCount);
	switch (collider.getColliderType())
	{
	case ColliderType::Static:
		staticColliders.push_back(collider);
		break;
	case ColliderType::Rigidbody:
		rigidbodyColliders.push_back(collider);
		break;
	default:
		break;
	}
}

void Collision::CollisionSystem::CheckCollisions()
{
	//sort static colliders by each type of mesh
	//sort rigidbody colliders by each type of mesh
	//test all static colliders against rigidbody colliders
	//test all rigidbody colliders against themselves

	//Notice how I never check if a static collider is colliding with another static collider
	//We do this, since a static collider will never move.
	//So, if they will never move, meaning they should never collide with another static
	//The only time this will happen is when the design places one atop the other
		//Not going to worry about that right now

	//for now we will try this:
	std::vector<Collider*> staticAABB;
	std::vector<Collider*> staticOBB;
	std::vector<Collider*> staticSphere;

	std::vector<Collider*> rgdbdyAABB;
	std::vector<Collider*> rgdbdyOBB;
	std::vector<Collider*> rgdbdySphere;

	for (int i = 0; i < staticColliders.size(); i++)
	{
		switch (staticColliders[i].getMeshType())
		{
		case MeshType::AABB:
			staticAABB.push_back(&staticColliders[i]);
			break;
		case MeshType::OBB:
			staticOBB.push_back(&staticColliders[i]);
			break;
		case MeshType::Sphere:
			staticSphere.push_back(&staticColliders[i]);
			break;
		}
	}

	for (int i = 0; i < rigidbodyColliders.size(); i++)
	{
		switch (rigidbodyColliders[i].getMeshType())
		{
		case MeshType::AABB:
			rgdbdyAABB.push_back(&rigidbodyColliders[i]);
			break;
		case MeshType::OBB:
			rgdbdyOBB.push_back(&rigidbodyColliders[i]);
			break;
		case MeshType::Sphere:
			rgdbdySphere.push_back(&rigidbodyColliders[i]);
			break;
		}
	}

	//Static AABB checks!!!!!!!!!!!!!!!!!!!!!!!!!!!

	for (int i = 0; i < staticAABB.size(); i++)
	{
		for(int j = 0; j < rgdbdyAABB.size(); j++)
		{
			if (AABB_AABB(*staticAABB[i], *rgdbdyAABB[j]))
				rgdbdyAABB[i]->onCollision();
		}
	}

	for (int i = 0; i < staticAABB.size(); i++)
	{
		for (int j = 0; j < rgdbdyOBB.size(); j++)
		{
			if (AABB_OBB(*staticAABB[i], *rgdbdyOBB[j]))
				rgdbdyOBB[i]->onCollision();
		}
	}

	for (int i = 0; i < staticAABB.size(); i++)
	{
		for (int j = 0; j < rgdbdySphere.size(); j++)
		{
			if (Sphere_AABB(*rgdbdySphere[j], *staticAABB[i]))
				rgdbdySphere[i]->onCollision();
		}
	}

	//Static OBB checks!!!!!!!!!!!!!!!!!!!!!!!!!!!

	for (int i = 0; i < staticOBB.size(); i++)
	{
		for (int j = 0; j < rgdbdyAABB.size(); j++)
		{
			if (AABB_OBB(*rgdbdyAABB[j], *staticOBB[i]))
				rgdbdyAABB[i]->onCollision();
		}
	}

	for (int i = 0; i < staticOBB.size(); i++)
	{
		for (int j = 0; j < rgdbdyOBB.size(); j++)
		{
			if (OBB_OBB(*staticOBB[i], *rgdbdyOBB[j]))
				rgdbdyOBB[i]->onCollision();
		}
	}

	for (int i = 0; i < staticOBB.size(); i++)
	{
		for (int j = 0; j < rgdbdySphere.size(); j++)
		{
			if (Sphere_OBB(*rgdbdySphere[j], *staticOBB[i]))
				rgdbdySphere[i]->onCollision();
		}
	}

	//Static Sphere checks!!!!!!!!!!!!!!!!!!!!!!!!!!!

	for (int i = 0; i < staticSphere.size(); i++)
	{
		for (int j = 0; j < rgdbdyAABB.size(); j++)
		{
			if (Sphere_AABB(*staticSphere[i], *rgdbdyAABB[j]))
				rgdbdyAABB[i]->onCollision();
		}
	}

	for (int i = 0; i < staticSphere.size(); i++)
	{
		for (int j = 0; j < rgdbdyOBB.size(); j++)
		{
			if (Sphere_OBB(*staticSphere[i], *rgdbdyOBB[j]))
				rgdbdyOBB[i]->onCollision();
		}
	}

	for (int i = 0; i < staticSphere.size(); i++)
	{
		for (int j = 0; j < rgdbdySphere.size(); j++)
		{
			if (Sphere_Sphere(*rgdbdySphere[j], *staticSphere[i]))
				rgdbdySphere[i]->onCollision();
		}
	}

	//Rigidbody Checks against themselves

	for (int i = 0; i < rgdbdyAABB.size() - 1; i++)
	{
		for (int j = i+1; j < rgdbdyAABB.size(); j++)
		{
			if (AABB_AABB(*rgdbdyAABB[i], *rgdbdyAABB[j]))
			{
				rgdbdyAABB[i]->onCollision();
				rgdbdyAABB[j]->onCollision();
			}
		}
	}

	for (int i = 0; i < rgdbdyOBB.size() - 1; i++)
	{
		for (int j = i + 1; j < rgdbdyOBB.size(); j++)
		{
			if (Sphere_OBB(*rgdbdyOBB[i], *rgdbdyOBB[j]))
				
			{
				rgdbdyOBB[i]->onCollision();
				rgdbdyOBB[j]->onCollision();
			}
		}
	}

	for (int i = 0; i < rgdbdySphere.size() - 1; i++)
	{
		for (int j = i + 1; j < rgdbdySphere.size(); j++)
		{
			if (Sphere_Sphere(*rgdbdySphere[i], *rgdbdySphere[j]))
			{
				rgdbdySphere[i]->onCollision();
				rgdbdySphere[j]->onCollision();
			}
		}
	}
}

bool Collision::Sphere_AABB(Collider a, Collider b)
{

	return false;
}

bool Collision::Sphere_Sphere(Collider a, Collider b)
{
	return false;
}

bool Collision::Sphere_OBB(Collider a, Collider b)
{
	return false;
}

bool Collision::AABB_AABB(Collider a, Collider b)
{
	AABBMesh a1 = *(AABBMesh*)&a.getMesh();
	AABBMesh a2 = *(AABBMesh*)&b.getMesh();
	if (a1.max.x < a1.min.x || a1.min.x > a2.max.x) return 0;
	if (a1.max.y < a1.min.y || a1.min.y > a2.max.y) return 0;
	if (a1.max.z < a1.min.z || a1.min.z > a2.max.z) return 0;
	return 1;
}

bool Collision::AABB_OBB(Collider a, Collider b)
{
	OBBMesh a1 = *(OBBMesh*)&a.getMesh();
	OBBMesh a2 = *(OBBMesh*)&b.getMesh();
	return false;
}

bool Collision::OBB_OBB(Collider a, Collider b)
{
	return false;
}
