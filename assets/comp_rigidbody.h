#ifndef COMP_RIGIDBODY
#define COMP_RIGIDBODY

#include "ecs.h"
#include "vector3.h"
#include "comp_collider.h"

class Rigidbody : public Component {
	private:
		struct Vector3 velocity;
		Collider* collider;
		float totalTime;
	public:
		virtual void awake();
		virtual void start();
		virtual void update(float delta);

		Rigidbody(int id, Entity* parent):Component(id, parent) {};
};

#endif