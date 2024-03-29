#ifndef COMP_CAMERA
#define COMP_CAMERA

#include "ecs.h"

class Camera : public Component {
	public:
		float dim;
		float fov;
		virtual void awake();
		Camera(int id, Entity* parent):Component(id, parent) {};
};

#endif