#pragma once
#include <vector>

#include "CollisionTypes.h"
#include "Collider.h"

//AABB:		stores minx, miny, minz, maxx, maxy, maxz
//OBB:		stores
//sphere:	stores

namespace Collision
{
	bool Sphere_AABB(Collider* a, Collider* b);
	bool Sphere_Sphere(Collider* a, Collider* b);
	bool Sphere_OBB(Collider* a, Collider* b);
	bool AABB_AABB(Collider* a, Collider* b);
	bool AABB_OBB(Collider* a, Collider* b);
	bool OBB_OBB(Collider* a, Collider* b);
	bool SBSphere_Sphere(Collider* a, Collider* b);
	bool SBSphere_SBSphere(Collider* a, Collider* b);

	//for ARBB
	//bool realignAABB(Collider col);
	//bool realignAllAABBs();

	/*bool computeAABB(Collider collider);
	bool computeOBB(Collider collider);
	bool computeSphere(Collider collider);*/

	class CollisionSystem
	{
	public:
		CollisionSystem();
		~CollisionSystem();

		void BatchCollider(Collider* collider);
		void CheckCollisions();

	private:
		//std::vector<Collider> staticColliders;
		//std::vector<Collider> rigidbodyColliders;
		std::vector<Collider*> staticAABB;
		std::vector<Collider*> staticOBB;
		std::vector<Collider*> staticSphere;
		std::vector<Collider*> staticSBSphere;

		std::vector<Collider*> rgdbdyAABB;
		std::vector<Collider*> rgdbdyOBB;
		std::vector<Collider*> rgdbdySphere;
		std::vector<Collider*> rgdbdySBSphere;
		int idCount;
	};
}