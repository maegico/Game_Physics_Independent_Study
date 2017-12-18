#include "Collision.h"

using namespace DirectX;

//nice Website:
//https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection

Collision::CollisionSystem::CollisionSystem()
{
	idCount = 0;
}

Collision::CollisionSystem::~CollisionSystem()
{
}

void Collision::CollisionSystem::BatchCollider(Collider& collider)
{
	if (collider.getID() != -1) return;

	//ids don't work
	//need to make it, so the show collider type & mesh type 
	collider.setID(idCount++);
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

	int staticAABBSize = staticAABB.size();
	int staticOBBSize = staticOBB.size();
	int staticSphereSize = staticSphere.size();

	int rgdbdyAABBSize = rgdbdyAABB.size();
	int rgdbdyOBBSize = rgdbdyOBB.size();
	int rgdbdySphereSize = rgdbdySphere.size();

	for (int i = 0; i < staticAABBSize; i++)
	{
		for(int j = 0; j < rgdbdyAABBSize; j++)
		{
			if (AABB_AABB(staticAABB[i], rgdbdyAABB[j]))
				if(rgdbdyAABB[i].onCollision != nullptr) rgdbdyAABB[i].onCollision();
		}
	}

	for (int i = 0; i < staticAABBSize; i++)
	{
		for (int j = 0; j < rgdbdyOBBSize; j++)
		{
			if (AABB_OBB(staticAABB[i], rgdbdyOBB[j]))
				if (rgdbdyOBB[i].onCollision != nullptr) rgdbdyOBB[i].onCollision();
		}
	}

	for (int i = 0; i < staticAABBSize; i++)
	{
		for (int j = 0; j < rgdbdySphereSize; j++)
		{
			if (Sphere_AABB(rgdbdySphere[j], staticAABB[i]))
				if (rgdbdySphere[i].onCollision != nullptr) rgdbdySphere[i].onCollision();
		}
	}

	//Static OBB checks!!!!!!!!!!!!!!!!!!!!!!!!!!!

	for (int i = 0; i < staticOBBSize; i++)
	{
		for (int j = 0; j < rgdbdyAABBSize; j++)
		{
			if (AABB_OBB(rgdbdyAABB[j], staticOBB[i]))
				if (rgdbdyAABB[i].onCollision != nullptr) rgdbdyAABB[i].onCollision();
		}
	}

	for (int i = 0; i < staticOBBSize; i++)
	{
		for (int j = 0; j < rgdbdyOBBSize; j++)
		{
			if (OBB_OBB(staticOBB[i], rgdbdyOBB[j]))
				if (rgdbdyOBB[i].onCollision != nullptr) rgdbdyOBB[i].onCollision();
		}
	}

	for (int i = 0; i < staticOBBSize; i++)
	{
		for (int j = 0; j < rgdbdySphereSize; j++)
		{
			if (Sphere_OBB(rgdbdySphere[j], staticOBB[i]))
				if (rgdbdySphere[i].onCollision != nullptr) rgdbdySphere[i].onCollision();
		}
	}

	//Static Sphere checks!!!!!!!!!!!!!!!!!!!!!!!!!!!

	for (int i = 0; i < staticSphereSize; i++)
	{
		for (int j = 0; j < rgdbdyAABBSize; j++)
		{
			if (Sphere_AABB(staticSphere[i], rgdbdyAABB[j]))
				if (rgdbdyAABB[i].onCollision != nullptr) rgdbdyAABB[i].onCollision();
		}
	}

	for (int i = 0; i < staticSphereSize; i++)
	{
		for (int j = 0; j < rgdbdyOBBSize; j++)
		{
			if (Sphere_OBB(staticSphere[i], rgdbdyOBB[j]))
				if (rgdbdyOBB[i].onCollision != nullptr) rgdbdyOBB[i].onCollision();
		}
	}

	for (int i = 0; i < staticSphereSize; i++)
	{
		for (int j = 0; j < rgdbdySphereSize; j++)
		{
			if (Sphere_Sphere(rgdbdySphere[j], staticSphere[i]))
				if (rgdbdySphere[i].onCollision != nullptr) rgdbdySphere[i].onCollision();
		}
	}

	//Rigidbody Checks against themselves
	
	if (rgdbdyAABBSize != 0)
	{
		for (int i = 0; i < rgdbdyAABBSize - 1; i++)
		{
			for (int j = i + 1; j < rgdbdyAABBSize; j++)
			{
				if (AABB_AABB(rgdbdyAABB[i], rgdbdyAABB[j]))
				{
					if (rgdbdyAABB[i].onCollision != nullptr) rgdbdyAABB[i].onCollision();
					if (rgdbdyAABB[j].onCollision != nullptr) rgdbdyAABB[j].onCollision();
				}
			}
		}
	}

	if (rgdbdyOBBSize != 0)
	{
		for (int i = 0; i < rgdbdyOBBSize - 1; i++)
		{
			for (int j = i + 1; j < rgdbdyOBBSize; j++)
			{
				if (OBB_OBB(rgdbdyOBB[i], rgdbdyOBB[j]))

				{
					if (rgdbdyOBB[i].onCollision != nullptr) rgdbdyOBB[i].onCollision();
					if (rgdbdyOBB[j].onCollision != nullptr) rgdbdyOBB[j].onCollision();
				}
			}
		}
	}

	if (rgdbdySphereSize != 0)
	{
		for (int i = 0; i < rgdbdySphereSize - 1; i++)
		{
			for (int j = i + 1; j < rgdbdySphereSize; j++)
			{
				if (Sphere_Sphere(rgdbdySphere[i], rgdbdySphere[j]))
				{
					if (rgdbdySphere[i].onCollision != nullptr) rgdbdySphere[i].onCollision();
					if (rgdbdySphere[j].onCollision != nullptr) rgdbdySphere[j].onCollision();
				}
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

	a1.vec1.x += a.transform.position.x;
	a1.vec1.y += a.transform.position.y;
	a1.vec1.z += a.transform.position.z;

	a2.vec1.x += b.transform.position.x;
	a2.vec1.y += b.transform.position.y;
	a2.vec1.z += b.transform.position.z;

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

	a1.vec1.x += a.transform.position.x;
	a1.vec1.y += a.transform.position.y;
	a1.vec1.z += a.transform.position.z;

	a1.vec2.x += a.transform.position.x;
	a1.vec2.y += a.transform.position.y;
	a1.vec2.z += a.transform.position.z;

	a2.vec1.x += b.transform.position.x;
	a2.vec1.y += b.transform.position.y;
	a2.vec1.z += b.transform.position.z;

	a2.vec2.x += b.transform.position.x;
	a2.vec2.y += b.transform.position.y;
	a2.vec2.z += b.transform.position.z;

	if (a1.vec2.x < a1.vec1.x || a1.vec1.x > a2.vec2.x) return 0;
	if (a1.vec2.y < a1.vec1.y || a1.vec1.y > a2.vec2.y) return 0;
	if (a1.vec2.z < a1.vec1.z || a1.vec1.z > a2.vec2.z) return 0;
	return 1;
}

//Collider a = AABB, Collider b = OBB
bool Collision::AABB_OBB(Collider aabb, Collider obb)
{
	return false;
}

bool Collision::OBB_OBB(Collider a, Collider b)
{
	//SAT
	ColliderMesh a1 = a.getMesh();
	ColliderMesh a2 = b.getMesh();

	a1.vec1.x += a.transform.position.x;
	a1.vec1.y += a.transform.position.y;
	a1.vec1.z += a.transform.position.z;

	a2.vec1.x += b.transform.position.x;
	a2.vec1.y += b.transform.position.y;
	a2.vec1.z += b.transform.position.z;

	float ra, rb;
	DirectX::XMFLOAT3X3 r, absR;

	//compute rotation matrix expressing b in a's coordinate frame
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
	t = XMFLOAT3(0,0,0);
	XMStoreFloat(&(t.x), dotT1);
	XMStoreFloat(&(t.y), dotT2);
	XMStoreFloat(&(t.z), dotT3);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			//using provided epsilon, not sure if this needs to be changed
			absR(i, j) = abs(r(i, j)) + FLT_EPSILON;
		}
	}

	//Test axes L = A0, L = A1, L = A2//
	ra = a1.vec2.x;
	rb = a2.vec2.x * absR(0,0) + a2.vec2.y * absR(0,1) + a2.vec2.z * absR(0, 2);
	if (abs(t.x) > ra + rb) return false;

	ra = a1.vec2.y;
	rb = a2.vec2.x * absR(1, 0) + a2.vec2.y * absR(1, 1) + a2.vec2.z * absR(1, 2);
	if (abs(t.y) > ra + rb) return false;

	ra = a1.vec2.z;
	rb = a2.vec2.x * absR(2, 0) + a2.vec2.y * absR(2, 1) + a2.vec2.z * absR(2, 2);
	if (abs(t.z) > ra + rb) return false;

	//Test axes L = B0, L = B1, L = B2//
	ra = a1.vec2.x * absR(0, 0) + a1.vec2.y * absR(1, 0) + a1.vec2.z * absR(2, 0);
	rb = a2.vec2.x;
	if (abs(t.x) > ra + rb) return false;

	ra = a1.vec2.x * absR(0, 1) + a1.vec2.y * absR(1, 1) + a1.vec2.z * absR(2, 1);
	rb = a2.vec2.y;
	if (abs(t.y) > ra + rb) return false;

	ra = a1.vec2.x * absR(0, 2) + a1.vec2.y * absR(1, 2) + a1.vec2.z * absR(2, 2);
	rb = a2.vec2.z;
	if (abs(t.z) > ra + rb) return false;

	//Test axis l = A0 x B0
	ra = a1.vec2.y * absR(2, 0) + a1.vec2.z * absR(1,0);
	rb = a2.vec2.y * absR(0, 2) + a2.vec2.z * absR(0,1);
	if (abs(t.z * r(1, 0) - t.y * r(2, 0)) > ra + rb) return false;

	//Test axis L = A0 x B1
	ra = a1.vec2.y * absR(2, 1) + a1.vec2.z * absR(1, 1);
	rb = a2.vec2.x * absR(0, 2) + a2.vec2.z * absR(0, 0);
	if (abs(t.z * r(1, 1) - t.y * r(2, 1)) > ra + rb) return false;

	//Test axis L = A0 x B2
	ra = a1.vec2.y * absR(2, 2) + a1.vec2.z * absR(1, 2);
	rb = a2.vec2.x * absR(0, 1) + a2.vec2.y * absR(0, 0);
	if (abs(t.z * r(1, 2) - t.y * r(2, 2)) > ra + rb) return false;

	//Test axis L = A1 x B0
	ra = a1.vec2.x * absR(2, 0) + a1.vec2.z * absR(0, 0);
	rb = a2.vec2.y * absR(1, 2) + a2.vec2.z * absR(1, 1);
	if (abs(t.x * r(2, 0) - t.z * r(0, 0)) > ra + rb) return false;

	//Test axis L = A1 x B1
	ra = a1.vec2.x * absR(2, 1) + a1.vec2.z * absR(0, 1);
	rb = a2.vec2.x * absR(1, 2) + a2.vec2.z * absR(1, 0);
	if (abs(t.x * r(2, 1) - t.z * r(0, 1)) > ra + rb) return false;

	//Test axis L = A1 x B2
	ra = a1.vec2.x * absR(2, 2) + a1.vec2.z * absR(0, 2);
	rb = a2.vec2.x * absR(1, 1) + a2.vec2.y * absR(1, 0);
	if (abs(t.x * r(2, 2) - t.z * r(0, 2)) > ra + rb) return false;

	//Test axis L = A2 x B0
	ra = a1.vec2.x * absR(1, 0) + a1.vec2.y * absR(0, 0);
	rb = a2.vec2.y * absR(2, 2) + a2.vec2.z * absR(2, 1);
	if (abs(t.y * r(0, 0) - t.x * r(1, 0)) > ra + rb) return false;

	//Test axis L = A2 x B1
	ra = a1.vec2.x * absR(1, 1) + a1.vec2.y * absR(0, 1);
	rb = a2.vec2.x * absR(2, 2) + a2.vec2.z * absR(2, 0);
	if (abs(t.y * r(0, 1) - t.x * r(1, 1)) > ra + rb) return false;

	//Test axis L = A2 x B2
	ra = a1.vec2.x * absR(1, 2) + a1.vec2.y * absR(0, 2);
	rb = a2.vec2.x * absR(2, 1) + a2.vec2.y * absR(2, 0);
	if (abs(t.y * r(0, 2) - t.x * r(1, 2)) > ra + rb) return false;

	return true;
}