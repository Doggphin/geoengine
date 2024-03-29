#include "comp_sawblade.h"
#include "ecs.h"

void Sawblade::awake() {
    totalTime = 0;
    centerpoint = new Vector3(0,0,0);
    mesh = NULL;
}

void Sawblade::start() {
    mesh = entity->getComponent<Mesh>();
}

void Sawblade::update(float delta) {
    totalTime += delta;
    Quaternion newDirection; Quaternion::fromToRotation(new Vector3(0, 0, -1), new Vector3(0, cos(totalTime * 12), sin(totalTime * 12)), &newDirection);
    entity->transform->position = centerpoint + new Vector3(0, 0, sin(totalTime) * 5);
    entity->transform->rotation = newDirection;
}