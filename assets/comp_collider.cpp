#include "comp_collider.h"
#include "macros.h"
#include "ecs.h"
#include "geometry.h"

void Collider::updateAABB() {
	// HOW TO CHECK IF SOMETHING IS EASILY VISIBLE:
	// - draw scene from point as white; draw visible thing in black;
	// find out how black the center area of the screen is; if pretty
	// black, then you can be seen
	// anyways this has nothing to do with colliders.

	// Run checks to see if AABB needs to be updated at all
	if(colliderShapeType == SHAPE_NONE) {
		return;
	}
	struct TransformFields currentFields;
	entity->transform->hierarchyTransform(&currentFields);
	if(fieldsCache.position != currentFields.position ||
	fieldsCache.rotation != currentFields.rotation ||
	fieldsCache.scale != currentFields.scale) {
		fieldsCache = currentFields;
		colliderShape->generateAABB(&currentFields, colliderAABBTransformed);
	}
	//colliderShape->generateAABB(colliderAABB);
}

void Collider::getTransformFields(TransformFields* output) {
	entity->transform->hierarchyTransform(output);
}

void Collider::awake() {
	colliderShapeType = SHAPE_NONE;
	colliderShape = new EmptyShape();
	colliderAABB = new AABB(new Vector3(0,0,0), new Vector3(0,0,0));
	colliderAABBTransformed = new AABB(new Vector3(0,0,0), new Vector3(0,0,0));
	layer = 2147483647;
	drawGizmo = 1;
}