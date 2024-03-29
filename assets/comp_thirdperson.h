#ifndef COMP_THIRDPERSON
#define COMP_THIRDPERSON

#include "ecs.h"

class thirdperson : public Component {
	public:
		thirdperson(int id, Entity* parent):Component(id, parent) {};
};

#endif