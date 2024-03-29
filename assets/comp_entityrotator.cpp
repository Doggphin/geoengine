#include "comp_entityrotator.h"
#include "ecs.h"

void EntityRotator::awake() {
	timeOffset = 0;
}

void EntityRotator::start() {
	totalTime = 0;
	totalTime -= timeOffset;
	m = entity->getComponent<Mesh>();
}

void EntityRotator::update(float delta) {
	totalTime += delta;

	if (m != NULL) {
		m->color = Vector3(sin(totalTime) / 2 + .5, cos(totalTime / 1.414) / 2 + .5, cos(totalTime)  / 2 + .5);
		m->color.normalize();
	}
    Vector3 rotator = Vector3(cos(totalTime), cos(totalTime*2), sin(totalTime));
	//Vector3 rotator = Vector3(cos(totalTime), 0, 0) + Vector3Forward;
	Vector3 forward = Vector3Forward;
    Quaternion::fromToRotation(&forward, &rotator, &(entity->transform->rotation));

	entity->transform->position = new Vector3(cos(totalTime / 3), sin(totalTime * 1.2) / 5, sin(totalTime / 3));
}