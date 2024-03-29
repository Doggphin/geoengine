# geoengine

My first attempt at a game engine, made with C++ and OpenGL 2, made as a final project for my computer graphics course in Fall 2023.

Includes an Entity-Component System architecture, .gltf model and texture loader, primitive shape raycasting and AABB optimizations.

Example component code:
```
#include "comp_sawblade.h"

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
```

Example entity code:
```
Entity* sawblade = ECS::instantiate();
		sawblade->transform->scale = new Vector3(0.2, 0.2, 0.2);
		Mesh* sawblade_mesh = sawblade->addComponent<Mesh>();
			GLTFLoader::loadMesh("killer.gltf", sawblade_mesh->gltfdata);
			sawblade_mesh->texture_id = GLTFLoader::loadBMPTex("Saw.bmp");
			sawblade_mesh->mesh_type = MESHTYPE_CUSTOM;
		Collider* sawblade_collider = sawblade->addComponent<Collider>();
			sawblade_collider->drawGizmo = false;
			sawblade_collider->colliderShape = new Cuboid(new Vector3(1.3, 7.4, 7.4));
			sawblade_collider->colliderShapeType = SHAPE_CUBE;
		Sawblade* sawblade_sawblade = sawblade->addComponent<Sawblade>();
			sawblade_sawblade->centerpoint = new Vector3(4, -1.9, 0);
		sawblade->name = "sawblade of death";
```

## Instructions:

Controls:
 - Move around with WASD.
 - Jump with Q.
 - Interact with E.

To compile and run:
```
make

./geoengine.exe
```

## Videos:
![Idle](visuals/video2.gif)

![Collisions](visuals/video1.gif)
