#ifndef COMP_SAWBLADE
#define COMP_SAWBLADE

#include "ecs.h"
#include "comp_mesh.h"

class Sawblade : public Component {
	private:
		Mesh* mesh;
		float totalTime;
	public:
		Vector3 centerpoint;
		Sawblade(int id, Entity* parent):Component(id, parent) {};
		virtual void awake();
		virtual void start();
		virtual void update(float delta);
};

#endif