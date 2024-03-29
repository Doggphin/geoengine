#include "comp_neonsign.h"
#include "ecs.h"

void NeonSign::awake() {
    totalTime = 0;
    offtexture = 0;
    ontexture = 0;
    mesh = NULL;
}

void NeonSign::start() {
    mesh = entity->getComponent<Mesh>();
}

void NeonSign::update(float delta) {
    totalTime += delta;
    Quaternion newDirection; Quaternion::lookRotation(new Vector3(sin(totalTime) * 0.3, 0, -1), new Vector3(sin(totalTime * 1.7) * 0.2, 1, cos(totalTime * 1.7) * 0.2), &newDirection);
    entity->transform->rotation = newDirection;

    if(mesh != NULL) {
        if(fmod(totalTime, 2) > 1) {
            mesh->texture_id = offtexture;
        } else {
            mesh->texture_id = ontexture;
        }
    }
}