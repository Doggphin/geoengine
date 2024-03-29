#include "comp_cameracontroller.h"
#include "ecs.h"
#include "vector2.h"
#include "input.h"

void CameraController::awake() {
	camera = NULL;
	totalMouseDelta = new Vector2();
}

void CameraController::start() {
	camera = entity->getComponent<Camera>();
	if (camera == NULL) { printf("No camera found!"); }
}

void CameraController::getLookVector(Vector3* out) {
	*out = new Vector3(cos(totalMouseDelta.x), -tan(totalMouseDelta.y), sin(totalMouseDelta.x));
}

void CameraController::apply() {
	struct Vector2 mouseDelta;
	Input::getMouseDelta(&mouseDelta);
	mouseDelta *= 0.005;
	totalMouseDelta += mouseDelta;
	totalMouseDelta = new Vector2(fmodf(totalMouseDelta.x, 6.2831), totalMouseDelta.y);
	if(totalMouseDelta.y > 1.5707) { totalMouseDelta = new Vector2(totalMouseDelta.x, 1.5707); }
	else if(totalMouseDelta.y < -1.5707) { totalMouseDelta = new Vector2(totalMouseDelta.x, -1.5707); }
	struct Vector3 lookVector;
	getLookVector(&lookVector);
	lookVector.normalize();
	struct Quaternion quat;
	struct Vector3 forward;
	forward = Vector3Forward;
	Quaternion::fromToRotation(&forward, &lookVector, &quat);
	entity->transform->rotation = &quat;
}