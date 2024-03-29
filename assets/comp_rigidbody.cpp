#include "comp_rigidbody.h"
#include "geometry.h"
#include "physics.h"
#include "ecs.h"
#include "comp_collider.h"

struct Vector3 gravity = {0, -.98, 0};

void Rigidbody::awake() {
	velocity = new Vector3(-.00005, -.000003, 0.0004);
}

void Rigidbody::start() {
	collider = entity->getComponent<Collider>();
}

void Rigidbody::update(float delta) {
	totalTime += delta;
	Vector3 gravityAcceleration = Vector3{0,0,0} - entity->transform->position;
	float distance = gravityAcceleration.magnitude();
	gravityAcceleration *= 1/distance * 0.000001;
	//Vector3_multiply(&gravityAcceleration, delta * 0.001, &gravityAcceleration);
	//Vector3_addf(&gravityAcceleration, sin(c->totalTime) * delta * 0.0001, sin(c->totalTime * 1.7071)* delta * 0.0001, cos(c->totalTime)* delta * 0.0001);
	velocity += gravityAcceleration * 0.05;

	/*

	Fix me later!

    Collider* collisions = NULL;
	collisions = Physics_findOverlappingAABBs(c->collider, (Collider*)ECS_getAllInstancesOfComponent(CTYPE_COLLIDER));
	struct Collider* possibleCollision;

	for(possibleCollision = collisions; possibleCollision != NULL; possibleCollision = possibleCollision->collisionhh.next) {
		struct CollisionInfo collisionInfo;
		if(Physics_getCollisionInfo(c->collider, possibleCollision, &collisionInfo) == 1) {
			Vector3_copy(&collisionInfo.safePosition, &c->entity->transform->position);
			float speed = Vector3_magnitude(&c->velocity);
			Vector3_copy(&collisionInfo.normal, &c->velocity);
			Vector3_multiply(&c->velocity, speed, &c->velocity);
			Vector3_multiply(&c->velocity, 0.99, &c->velocity);
			//struct Vector3 test;
			//Vector3_preset(VECTOR3_UP, &test);
			//Vector3_multiply(&test, 0.001, &test);
			//Vector3_copy(&test, &c->velocity);
		}
	}

	//printf("Moving by ");
	//Vector3_print(&c->velocity);
    Vector3_add(&c->velocity, &c->entity->transform->position, &c->entity->transform->position);

	*/
}
/*template <class T> T* Entity::getComponent() {
	return ECS::getComponentByParent<T>(this);
}*/

//template T* Entity::getComponent<Rigidbody*>();