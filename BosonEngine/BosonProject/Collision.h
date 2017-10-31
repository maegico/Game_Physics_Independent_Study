#pragma once
#include "ColliderEnums.h"
#include "Collider.h"

namespace Collision
{
	bool Sphere_AABB();
	bool Sphere_Sphere();
	bool Sphere_OBB();
	bool AABB_AABB();
	bool AABB_OBB();
	bool OBB_OBB();
}