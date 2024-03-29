#ifndef COMP_ENTITYROTATOR
#define COMP_ENTITYROTATOR

#include "ecs.h"
#include "comp_mesh.h"

class EntityRotator : public Component {
	private:
		float totalTime;
		Mesh* m;
	public:
		float timeOffset;
		virtual void awake();
		virtual void start();
		virtual void update(float delta);

		EntityRotator(int id, Entity* parent):Component(id, parent) {};
};

#endif