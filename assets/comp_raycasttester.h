#ifndef COMP_RAYCASTTESTER_H
#define COMP_RAYCASTTESTER_H

#include "ecs.h"
#include "comp_mesh.h"

class RaycastTester : public Component {
	public:
		Mesh* testmesh;
		RaycastTester(int id, Entity* parent):Component(id, parent) {};

		virtual void lateupdate(float delta);
		virtual void update(float delta);
		virtual void awake();
};

#endif