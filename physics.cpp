#include "physics.h"
#include <assert.h>
#include <list>
#include <unordered_map>
#include "ecs.h"
#include "geometry.h"
#include "gizmodrawer.h"
#include <iostream>

int Physics::penetrationSphereInSphere(Sphere* sphereDynamic, TransformFields* dyn, Sphere* sphereStatic, TransformFields* stat, CollisionInfo* out) {
	assert(out != NULL);

	struct Vector3 dynamicToStatic;
	dynamicToStatic = stat->position - dyn->position;
	float sphereDynamicRadius = sphereDynamic->radius * dyn->scale.max();
	float sphereStaticRadius = sphereStatic->radius * stat->scale.max();
	float cumradius = sphereDynamicRadius + sphereStaticRadius;
	float distance = dynamicToStatic.magnitude();

	//printf("Distance is %.6f, cumulative radius is %.6f\n", distance, cumradius);
	if (distance >= cumradius) { return 0; }
	// First, set distance of out distance
	//out->distance = distance;

	// Next, generate normalized fromTo and assign out normal to its negative
	struct Vector3 fromTo = dynamicToStatic;
	dynamicToStatic.normalize();
	out->normal = fromTo;
	out->normal *= -1;

	// Set depth to distance minus their radii
	//out->depth = distance - cumradius;

	// Set safeposition to dynamic sphere's center minus depth * the negative fromTo vector
	//out->safePosition = &fromTo;
	//out->safePosition *= out->depth - sphereDynamic->skin;
	//out->safePosition += &sphereDynamic->center;

	// Set point to bounded point trying to go from safeposition to static sphere's center
	out->point = fromTo * sphereDynamicRadius;
	//out->point += &out->safePosition;

	return 1;
}

void Physics::findOverlappingAABBs(AABB* aabb, std::list<Collider*>* overlaps, int layerMask) {
	for(auto i : ECS::hashes[typeid(Collider*).hash_code()]) {
		Collider* check = dynamic_cast<Collider*>(i.second);
		if((layerMask & check->layer) != 0) {
			if(check->colliderAABBTransformed != aabb && Geometry::AABBIsInAABB(aabb, check->colliderAABBTransformed)) {
			overlaps->push_back(check);
			}
		}
	}
}

void Physics::findOverlaps(Collider* collider, std::list<Collider*>* overlaps) {

}

int Physics::computePenetration(Collider* a, Collider* b, CollisionInfo* output) {
	struct TransformFields atf; a->getTransformFields(&atf);
	struct TransformFields btf; b->getTransformFields(&btf);
	switch(a->colliderShapeType) {
		case SHAPE_SPHERE:
			switch(b->colliderShapeType) {
				case SHAPE_SPHERE:
					return Physics::penetrationSphereInSphere(static_cast<Sphere*>(a->colliderShape), &atf, static_cast<Sphere*>(b->colliderShape), &btf, output);
				default:
					printf("Unimplemented collision detection for shape types %d and %d!\n", a->colliderShapeType, b->colliderShapeType); return 0;
			}
		default:
			printf("Unimplemented collision detection for shape types %d and %d!\n", a->colliderShapeType, b->colliderShapeType); return 0;
	}
}

//https://iquilezles.org/articles/intersectors/
Vector2 Physics::raycastSphere(Vector3* from, Vector3* direction, Vector3* sphereCenter, float radius) {
	Vector3 toSphere = *from - sphereCenter;
	float b = Vector3::dot(&toSphere, direction);
	float c = Vector3::dot(&toSphere, &toSphere) - radius * radius;
	float h = b*b - c;
	if(h<0) {
		return new Vector2(-1, -1);
	} else {
		h = sqrt(h);
		return new Vector2(-b-h, -b+h);
	}
}

//https://iquilezles.org/articles/intersectors/
float Physics::raycastCapsule(Vector3* from, Vector3* direction, Vector3* pa, Vector3* pb, float radius) {
	Vector3 ba = *pb - *pa;
	Vector3 oa = *direction - *pa;
	float baba = Vector3::dot(&ba, &ba);
	float bard = Vector3::dot(&ba, direction);
	float baoa = Vector3::dot(&ba, &oa);
	float rdoa = Vector3::dot(direction, &oa);
	float oaoa = Vector3::dot(&oa, &oa);
	float a = baba - bard * bard;
	float b = baba * rdoa - baoa * bard;
	float c = baba * oaoa - baoa*baoa - radius*radius*baba;
	float h = b*b - a*c;
	if(h>=0.0) {
		float t = abs((-b-sqrt(h)) / a);
		float y = baoa + t * bard;
		// body
		if ( y>0.0 && y<baba) { return t; }
		Vector3 oc = (y <= 0.0) ? oa : *direction - pb;
		b = Vector3::dot(direction, &oc);
		c = Vector3::dot(&oc, &oc) - radius * radius;
		h = b*b - c;
		if(h>0.0) { return abs(-b - sqrt(h)); }
	}
	return -1;
}

Vector2 Physics::raycastBox(Vector3* from, Vector3* to, Vector3* boxMin, Vector3* boxMax, RaycastResults* results1, RaycastResults* results2) {
	struct Vector3 point1;
	struct Vector3 point2;
	char code1;
	char code2;
	if (!Geometry::clipLine(boxMin, boxMax, from, to, &point1, &point2, &code1, &code2, false, false))
	{
		return Vector2(-1, -1);
	}
	else {
		for(int i=0; i<=1; i++) {
			Vector3* normalOut = i == 0 ? &results1->normal : &results2->normal;
			char code = i == 0 ? code1 : code2;
			if(code == 1) {
				normalOut = new Vector3(-1, 0, 0);
			}
			else if(code == 2) {
				normalOut = new Vector3(1, 0, 0);
			}
			else if(code == 4) {
				normalOut = new Vector3(0, -1, 0);
			}
			else if(code == 8) {
				normalOut = new Vector3(0, 1, 0);
			}
			else if(code == 16) {
				normalOut = new Vector3(0, 0, -1);
			}
			else if(code == 32) {
				normalOut = new Vector3(0, 0, 1);
			}
		}
		results1->point = point1;
		results2->point = point2;
		results1->distance = (point1 - from).magnitude();
		results2->distance = (point2 - from).magnitude();
		return Vector2(results1->distance, results2->distance);
	}
}

int Physics::raycast(Vector3* from, Vector3* dir, float time, int layerMask, RaycastResults* out) {
	struct Vector3 to = *from + (*dir * time);
	struct Vector3 cumMin; struct Vector3 cumMax; 
	Vector3::cumMin(from, &to, &cumMin);
	Vector3::cumMax(from, &to, &cumMax);
	struct AABB raycastAABB(&cumMax, &cumMin);
	std::list<Collider*> overlaps;
	Physics::findOverlappingAABBs(&raycastAABB, &overlaps, layerMask);

	struct RaycastResults results;
	struct RaycastResults results2;
	struct TransformFields t;
	float currentMinT = 1000000;	// really big number 
	int hit = 0;
	for(Collider* i : overlaps) {
		i->getTransformFields(&t);
		switch(i->colliderShapeType) {
			case SHAPE_SPHERE: {
				Sphere* sphere = static_cast<Sphere*>(i->colliderShape);
				float sphereRadius = sphere->radius * t.scale.max();
				struct Vector2 sphereOut = Physics::raycastSphere(from, dir, &t.position, sphereRadius);
				if(sphereOut.x != -1 && (sphereOut.x < currentMinT || hit == 0)) {
					hit = 1;
					currentMinT = 0;
					results.collider = i;
					results.distance = sphereOut.x;
					results.point = *from + (*dir * sphereOut.x);
					results.normal = results.point - t.position;
					if(sphereRadius != 1) {
						results.normal /= sphereRadius;
					}
				}
			} break;
			case SHAPE_CUBE: {
				Cuboid* cube = static_cast<Cuboid*>(i->colliderShape);
				struct Vector3 cubeScale = t.scale * cube->scale;
				struct Vector3 cubeMin = t.position - (cubeScale * .5);
				struct Vector3 cubeMax = t.position + (cubeScale * .5);
				struct Vector3 unrotatedFrom = *from - t.position;
				struct Vector3 unrotatedTo = to - t.position;
				struct Quaternion conjugate;

				Quaternion::conjugate(&t.rotation, &conjugate);
				Quaternion::rotate(&conjugate, &unrotatedFrom, &unrotatedFrom);
				Quaternion::rotate(&conjugate, &unrotatedTo, &unrotatedTo);

				unrotatedFrom += t.position;
				unrotatedTo += t.position;
				struct RaycastResults temp1;
				struct RaycastResults temp2;
				struct Vector2 boxOut = raycastBox(&unrotatedFrom, &unrotatedTo, &cubeMin, &cubeMax, &temp1, &temp2);
				if(boxOut.x != -1 && (boxOut.x < currentMinT || hit == 0)) {
					temp1.point -= t.position;
					Quaternion::rotate(&t.rotation, &temp1.point, &results.point);
					results.point += t.position;
					results.collider = i;
					Quaternion::rotate(&t.rotation, &temp1.normal, &results.normal);
					results.distance = boxOut.x;
					currentMinT = boxOut.x;
					hit = 1;
				}
			} break;
			case SHAPE_CAPSULE: {
				Capsule* capsule = static_cast<Capsule*>(i->colliderShape);
				struct LineSegment axis;
				capsule->getAxis(&axis);
				struct Vector3 start = axis.start - t.position;
				struct Vector3 end = axis.end - t.position;
				float distance = Physics::raycastCapsule(from, dir, &start, &end, capsule->radius);
				if(distance != -1 && (distance < currentMinT || hit == 0)) {
					results.point = *from + (*dir * distance);
					results.collider = i;
					results.distance = distance;
					hit = 1;
				} else {
					//printf("didn't hit it\n");
				}
			} break;
			case SHAPE_AABB: {
				AABB* aabb = static_cast<AABB*>(i->colliderShape);
				struct RaycastResults aabbtemp1;
				struct RaycastResults aabbtemp2;
				struct Vector2 aabbOut = raycastBox(from, &to, &aabb->min, &aabb->max, &aabbtemp1, &aabbtemp2);
				if(aabbOut.x != -1 && (aabbOut.x < currentMinT || hit == 0)) {
					results = aabbtemp1;
					currentMinT = aabbOut.x;
					results.collider = i;
					hit = 1;
				}
			} break;
			default: {

			} break;
				
		}
	}
	*out = results;
	return hit;
}
int Physics::raycast(Vector3* from, Vector3* to, int layerMask, RaycastResults* out) {
	struct Vector3 direction = *to - from;
	float directionmag = direction.magnitude();
	direction /= directionmag;
	return Physics::raycast(from, &direction, directionmag, layerMask, out);
}

void Physics::updateAABBs() {
	int colls=0;
	for(auto i : ECS::hashes[typeid(Collider*).hash_code()]) {
		Collider* collider = static_cast<Collider*>(i.second);
		collider->updateAABB();
		colls++;
	}
}
