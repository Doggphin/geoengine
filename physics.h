#ifndef PHYSICS
#define PHYSICS

#include "vector3.h"
#include "vector2.h"
#include "comp_collider.h"
#include "ecs.h"
#include <unordered_map>
#include <list>
#include <vector>

typedef struct CollisionInfo {
	Vector3 point;
	Vector3 normal;
} CollisionInfo;

typedef struct RaycastResults {
	Collider* collider;
	Vector3 point;
	Vector3 normal;
	float distance;
	void operator=(RaycastResults* a) {
		collider = a->collider;
		point = a->point;
		normal = a->normal;
		distance = a->distance;
	};
	void operator=(RaycastResults a) {
		*this = &a;
	}
} RaycastResults;

class Physics {
	private:
		static int penetrationSphereInSphere(Sphere* sphereDynamic, TransformFields* dyn, Sphere* sphereStatic, TransformFields* stat, CollisionInfo* out);
	public:
		static void updateAABBs();
		static void findOverlappingAABBs(AABB* aabb, std::list<Collider*>* overlaps, int layerMask);
		static void findOverlaps(Collider* collider, std::list<Collider*>* overlaps);

		static Vector2 raycastSphere(Vector3* from, Vector3* direction, Vector3* sphereCenter, float radius);
		static Vector2 raycastBox(Vector3* from, Vector3* direction, Vector3* boxCenter, Vector3* scale, RaycastResults* results1, RaycastResults* results2);
		static float raycastCapsule(Vector3* from, Vector3* direction, Vector3* pa, Vector3* pb, float radius);
		static int raycast(Vector3* from, Vector3* dir, float time, int layerMask, RaycastResults* out);
		static int raycast(Vector3* from, Vector3* to, int layerMask, RaycastResults* out);

		static std::list<CollisionInfo*> overlapSphere(Vector3* position, float radius, int layer);
		static std::list<CollisionInfo*> overlapCapsule(Vector3* point0, Vector3* point1, float radius, int layer);
		static int computePenetration(Collider* a, Collider* b, CollisionInfo* out);
};

#endif