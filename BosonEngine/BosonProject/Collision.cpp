#include "Collision.h"

using namespace DirectX;

//nice Website:
//https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection

Collision::CollisionSystem::CollisionSystem()
{
}

Collision::CollisionSystem::~CollisionSystem()
{
}

void Collision::CollisionSystem::BatchCollider(Collider& collider)
{
	if (collider.getID() == 0) return;

	//ids don't work
	//need to make it, so the show collider type & mesh type 
	collider.setID(++idCount);
	switch (collider.getColliderType())
	{
	case ColliderType::Static:
		switch (collider.getMeshType())
		{
		case MeshType::AABB:
			staticAABB.push_back(collider);
			break;
		case MeshType::OBB:
			staticOBB.push_back(collider);
			break;
		case MeshType::Sphere:
			staticSphere.push_back(collider);
			break;
		}
	case ColliderType::Rigidbody:
		switch (collider.getMeshType())
		{
		case MeshType::AABB:
			rgdbdyAABB.push_back(collider);
			break;
		case MeshType::OBB:
			rgdbdyOBB.push_back(collider);
			break;
		case MeshType::Sphere:
			rgdbdySphere.push_back(collider);
			break;
		}
	default:
		break;
	}

	//should also presort them by collider mesh type 
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

	//Static AABB checks!!!!!!!!!!!!!!!!!!!!!!!!!!!

	for (int i = 0; i < staticAABB.size(); i++)
	{
		for(int j = 0; j < rgdbdyAABB.size(); j++)
		{
			if (AABB_AABB(staticAABB[i], rgdbdyAABB[j]))
				rgdbdyAABB[i].onCollision();
		}
	}

	for (int i = 0; i < staticAABB.size(); i++)
	{
		for (int j = 0; j < rgdbdyOBB.size(); j++)
		{
			if (AABB_OBB(staticAABB[i], rgdbdyOBB[j]))
				rgdbdyOBB[i].onCollision();
		}
	}

	for (int i = 0; i < staticAABB.size(); i++)
	{
		for (int j = 0; j < rgdbdySphere.size(); j++)
		{
			if (Sphere_AABB(rgdbdySphere[j], staticAABB[i]))
				rgdbdySphere[i].onCollision();
		}
	}

	//Static OBB checks!!!!!!!!!!!!!!!!!!!!!!!!!!!

	for (int i = 0; i < staticOBB.size(); i++)
	{
		for (int j = 0; j < rgdbdyAABB.size(); j++)
		{
			if (AABB_OBB(rgdbdyAABB[j], staticOBB[i]))
				rgdbdyAABB[i].onCollision();
		}
	}

	for (int i = 0; i < staticOBB.size(); i++)
	{
		for (int j = 0; j < rgdbdyOBB.size(); j++)
		{
			if (OBB_OBB(staticOBB[i], rgdbdyOBB[j]))
				rgdbdyOBB[i].onCollision();
		}
	}

	for (int i = 0; i < staticOBB.size(); i++)
	{
		for (int j = 0; j < rgdbdySphere.size(); j++)
		{
			if (Sphere_OBB(rgdbdySphere[j], staticOBB[i]))
				rgdbdySphere[i].onCollision();
		}
	}

	//Static Sphere checks!!!!!!!!!!!!!!!!!!!!!!!!!!!

	for (int i = 0; i < staticSphere.size(); i++)
	{
		for (int j = 0; j < rgdbdyAABB.size(); j++)
		{
			if (Sphere_AABB(staticSphere[i], rgdbdyAABB[j]))
				rgdbdyAABB[i].onCollision();
		}
	}

	for (int i = 0; i < staticSphere.size(); i++)
	{
		for (int j = 0; j < rgdbdyOBB.size(); j++)
		{
			if (Sphere_OBB(staticSphere[i], rgdbdyOBB[j]))
				rgdbdyOBB[i].onCollision();
		}
	}

	for (int i = 0; i < staticSphere.size(); i++)
	{
		for (int j = 0; j < rgdbdySphere.size(); j++)
		{
			if (Sphere_Sphere(rgdbdySphere[j], staticSphere[i]))
				rgdbdySphere[i].onCollision();
		}
	}

	//Rigidbody Checks against themselves

	for (int i = 0; i < rgdbdyAABB.size() - 1; i++)
	{
		for (int j = i+1; j < rgdbdyAABB.size(); j++)
		{
			if (AABB_AABB(rgdbdyAABB[i], rgdbdyAABB[j]))
			{
				rgdbdyAABB[i].onCollision();
				rgdbdyAABB[j].onCollision();
			}
		}
	}

	for (int i = 0; i < rgdbdyOBB.size() - 1; i++)
	{
		for (int j = i + 1; j < rgdbdyOBB.size(); j++)
		{
			if (Sphere_OBB(rgdbdyOBB[i], rgdbdyOBB[j]))
				
			{
				rgdbdyOBB[i].onCollision();
				rgdbdyOBB[j].onCollision();
			}
		}
	}

	for (int i = 0; i < rgdbdySphere.size() - 1; i++)
	{
		for (int j = i + 1; j < rgdbdySphere.size(); j++)
		{
			if (Sphere_Sphere(rgdbdySphere[i], rgdbdySphere[j]))
			{
				rgdbdySphere[i].onCollision();
				rgdbdySphere[j].onCollision();
			}
		}
	}
}

bool Collision::Sphere_AABB(Collider sphere, Collider aabb)
{

	return false;
}

bool Collision::Sphere_Sphere(Collider a, Collider b)
{
	//vec1 = center
	//vec2.x = radius
	float distance;
	ColliderMesh a1 = a.getMesh();
	ColliderMesh a2 = b.getMesh();
	DirectX::XMVECTOR c1 = DirectX::XMLoadFloat3(&a1.vec1);
	DirectX::XMVECTOR c2 = DirectX::XMLoadFloat3(&a2.vec1);
	DirectX::XMVECTOR length = DirectX::XMVector3Length(c1 - c2);
	XMStoreFloat(&distance, length);

	return distance < a1.vec2.x + a2.vec2.x;
}

bool Collision::Sphere_OBB(Collider sphere, Collider obb)
{
	return false;
}

bool Collision::AABB_AABB(Collider a, Collider b)
{
	//vec1 = min, vec2 = max
	ColliderMesh a1 = a.getMesh();
	ColliderMesh a2 = b.getMesh();
	if (a1.vec2.x < a1.vec1.x || a1.vec1.x > a2.vec2.x) return 0;
	if (a1.vec2.y < a1.vec1.y || a1.vec1.y > a2.vec2.y) return 0;
	if (a1.vec2.z < a1.vec1.z || a1.vec1.z > a2.vec2.z) return 0;
	return 1;
}

//Collider a = AABB, Collider b = OBB
bool Collision::AABB_OBB(Collider aabb, Collider obb)
{
	ColliderMesh a1 = aabb.getMesh();
	ColliderMesh a2 = obb.getMesh();
	return false;
}

bool Collision::OBB_OBB(Collider a, Collider b)
{
	//SAT
	ColliderMesh a1 = a.getMesh();
	ColliderMesh a2 = b.getMesh();

	float ra, rb;
	DirectX::XMFLOAT3X3 r, absR;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			XMVECTOR vec1 = XMLoadFloat3(&a1.axes[i]);
			XMVECTOR vec2 = XMLoadFloat3(&a2.axes[j]);
			XMVECTOR dot = XMVector3Dot(vec1, vec2);
			XMStoreFloat(&r(i,j), dot);
		}
	}

	//Compute the translation vector
	XMFLOAT3 t;
	XMVECTOR translationVec = XMLoadFloat3(&a2.vec1) - XMLoadFloat3(&a1.vec1);
	//bring translation into a's coordinate frame
	//set t equal (dot of t & a's axis0, dot of t & a's axis1, dot of t & a's axis2)
	
	XMVECTOR t1 = XMLoadFloat3(&a1.axes[0]);
	XMVECTOR t2 = XMLoadFloat3(&a1.axes[1]);
	XMVECTOR t3 = XMLoadFloat3(&a1.axes[2]);

	XMVECTOR dotT1 = XMVector3Dot(translationVec, t1);
	XMVECTOR dotT2 = XMVector3Dot(translationVec, t2);
	XMVECTOR dotT3 = XMVector3Dot(translationVec, t3);



	return false;
}