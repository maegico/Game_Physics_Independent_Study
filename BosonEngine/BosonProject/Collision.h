#pragma once
#include <vector>

#include "ColliderEnums.h"
#include "Collider.h"

//AABB:		stores minx, miny, minz, maxx, maxy, maxz
//OBB:		stores
//sphere:	stores

namespace Collision
{
	bool Sphere_AABB(Collider a, Collider b);
	bool Sphere_Sphere(Collider a, Collider b);
	bool Sphere_OBB(Collider a, Collider b);
	bool AABB_AABB(Collider a, Collider b);
	bool AABB_OBB(Collider a, Collider b);
	bool OBB_OBB(Collider a, Collider b);

	/*bool computeAABB(Collider collider);
	bool computeOBB(Collider collider);
	bool computeSphere(Collider collider);*/

	class CollisionSystem
	{
	public:
		CollisionSystem();
		~CollisionSystem();

		void BatchCollider(Collider& collider);
		void CheckCollisions();

	private:
		std::vector<Collider> staticColliders;
		std::vector<Collider> rigidbodyColliders;
		int idCount;
	};
}