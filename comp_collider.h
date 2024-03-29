#ifndef COMP_COLLIDER_H
#define COMP_COLLIDER_H

#include "ecs.h"
#include "vector3.h"
#include "Quaternion.h"
#include "geometry.h"

class Collider : public Component {
	public:
		int drawGizmo;
		
		PrimitiveShape colliderShapeType;
		ShapeBase* colliderShape;

		AABB* colliderAABB;
		AABB* colliderAABBTransformed;
		TransformFields fieldsCache;

		int layer;

		virtual void awake();
		void updateAABB();

		Collider(int id, Entity* parent):Component(id, parent) {};
		void getTransformFields(TransformFields* output);
		void setShapeSphere(float radius);
};

#endif