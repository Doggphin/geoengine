#ifndef COMP_CAMERACONTROLLER
#define COMP_CAMERACONTROLLER

#include "comp_camera.h"
#include "vector2.h"

class CameraController : public Component {
	private:
		struct Vector2 totalMouseDelta;
		Camera* camera;
	public:
		virtual void awake();
		virtual void start();
		void apply();
		void getLookVector(Vector3* out);
		CameraController(int id, Entity* parent):Component(id, parent) {};
};

#endif