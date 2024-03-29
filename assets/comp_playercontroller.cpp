#include "comp_playercontroller.h"
#include "ecs.h"
#include "input.h"
#include "Quaternion.h"
#include "comp_collider.h"
#include "comp_sawblade.h"
#include "physics.h"
#include <stdlib.h>

void PlayerController::awake() {
	cc = NULL;
	collider = NULL;
	playerheight = 1;
	gravity = -16;
	movespeed = 25;
	jumpheight = 8;
	grounded = false;
	basedragfactor = 10;
	airdragmultiplier = .1;
	aircontrolmultiplier = .2;
}

void PlayerController::start() {
	cc = entity->getComponent<CameraController>();
	collider = entity->getComponent<Collider>();
	if(cc == NULL) {printf("PlayerController requires component CameraController to function!\n");}
	if(collider == NULL) {printf("PlayerController requires component Collider to function!\n");}
}

void PlayerController::update(float delta) {
	if(cc != NULL){
		cc->apply();
	}

	struct Vector2* joystick = new Vector2();
	Input::getJoystickNormalized(KEYCODE_W, KEYCODE_D, KEYCODE_S, KEYCODE_A, 1, joystick);
	Vector3 wasd = new Vector3(-joystick->x, 0, joystick->y);
	struct Quaternion rotation;
	struct Vector3 up = Vector3Up;
	struct Vector3 lookVector; cc->getLookVector(&lookVector);
	lookVector.y = 0;
	Quaternion::lookRotation(&lookVector, &up, &rotation);
	struct Vector3 finalMove;

	Quaternion::rotate(&rotation, &wasd, &finalMove);

	finalMove *= movespeed * (grounded ? 1 : aircontrolmultiplier);
	velocity = (finalMove * delta) + velocity;
	velocity -= new Vector3(0, -gravity * delta, 0);
	struct Vector3 drag = velocity * (basedragfactor * delta);
	if(grounded) {
		velocity -= (drag);
	} else {
		velocity -= (drag * airdragmultiplier);
	}
	
	entity->transform->position += velocity * delta;

	RaycastResults results;
	Vector3 target(entity->transform->position.x, entity->transform->position.y - 2, entity->transform->position.z);
	if(Physics::raycast(&entity->transform->position, &target, 2147483647, &results)) {
		if(results.distance <= playerheight) {
			grounded = true;
			velocity.y = 0;
			if(results.collider->entity->getComponent<Sawblade>() != NULL) { entity->transform->position = new Vector3(0, 1, 0); }
			entity->transform->position += Vector3(0,playerheight-results.distance,0);
			if(Input::getKeyBuffered(KEYCODE_Q) && grounded) {
				velocity = Vector3(velocity.x, jumpheight, velocity.z);
				entity->transform->position += Vector3(0, .01, 0);
				grounded = false;
			}
		}
	} else {
		grounded = false;
	}

	if(entity->transform->position.y < -20) {
		entity->transform->position = new Vector3(0,0,0);
	}
}