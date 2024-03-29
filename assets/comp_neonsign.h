#ifndef COMP_NEONSIGN
#define COMP_NEONSIGN

#include "ecs.h"
#include "comp_mesh.h"

class NeonSign : public Component {
	private:
		Mesh* mesh;
		float totalTime;
	public:
		unsigned int offtexture;
		unsigned int ontexture;
		NeonSign(int id, Entity* parent):Component(id, parent) {};
		virtual void awake();
		virtual void start();
		virtual void update(float delta);
};

#endif