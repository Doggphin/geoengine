#include "ecs.h"
#include "geometry.h"
//#include "physics.h"
#include "Quaternion.h"
#include <typeinfo>

using namespace std;

// https://gamedev.stackexchange.com/questions/172584/how-could-i-implement-an-ecs-in-c

void Component::kill() {
	ECS::destroyComponent(this);
}
Component::Component(int id, Entity* parentEntity) {
	this->id_ = id;
	this->entity = parentEntity;
	this->parent_id_ = parentEntity == NULL ? 0 : parentEntity->id;
	this->enabled_ = true;
	awake();
}

void Transform::awake() {
	position = Vector3Zero;
	scale = Vector3Zero;
	rotation = IdentityQuaternion;
	relative = true;
}

void Transform_copyData(Transform* t, Transform* output) {
	output->position = t->position;
	output->scale = t->scale;
	output->rotation = t->rotation;
}

void Transform::hierarchyScale(Vector3* output) {
	*output = scale;
	if (relative == 0 || entity->parent_id == 0) { return; }
	struct Transform* transformBuffer = entity->entity->transform;
	while (1) {
		if (transformBuffer->relative == 0 || transformBuffer->entity->parent_id == 0)
		{
			return;
		}
		else {
			*output *= &transformBuffer->scale;
			transformBuffer = transformBuffer->entity->transform;
		}
	}
}
void Transform::hierarchyRotation(Quaternion* output) {
	*output = rotation;
	if (relative == 0 || entity->parent_id == 0) { return; }
	struct Transform* transformBuffer = entity->entity->transform;
	while (1) {
		if (transformBuffer->relative == 0 || transformBuffer->entity->parent_id == 0) {
			return;
		}
		else {
			*output *= &transformBuffer->rotation;
			transformBuffer = transformBuffer->entity->transform;
		}
	}
}
void Transform::hierarchyPosition(Vector3* output) {
	*output = position;
	if (relative == 0 || entity->parent_id == 0) { return; }
	struct Transform* transformBuffer = entity->entity->transform;
	while (1) {
		if (transformBuffer->relative == 0 || transformBuffer->entity->parent_id == 0) {
			 return;
		}
		else {
			*output += &transformBuffer->position;
			transformBuffer = transformBuffer->entity->transform;
		}
	}
}
void Transform::hierarchyTransform(TransformFields* output) {
	hierarchyScale(&output->scale);
	hierarchyPosition(&output->position);
	hierarchyRotation(&output->rotation);
}

int ECS::component_id_counter = 0;
std::unordered_map<std::size_t, std::unordered_map<int, Component*>> ECS::hashes;
unordered_set<Component*> ECS::startsToRun;

Entity* ECS::createEntity(int id, Entity* parent) {
	Entity* ret = (Entity*)malloc(sizeof(Entity));
	ret = new (ret)Entity(id, parent);
	ret->name = "Entity " + to_string(id);
	ret->transform = ret->addComponent<Transform>();
	ret->transform->scale = Vector3One;
	ret->transform->rotation = IdentityQuaternion;
	hashes[typeid(Entity).hash_code()][id] = dynamic_cast<Component*>(ret);
	return ret;
}

Entity* ECS::instantiate() {
	component_id_counter++;
	return createEntity(component_id_counter, NULL);
}




Entity* ECS::getEntityById(int id) {
	return dynamic_cast<Entity*>(hashes.at(typeid(Entity).hash_code()).at(id));
}
bool ECS::destroyComponent(Component* component) {
	bool ret = (hashes[typeid(component).hash_code()]).erase(component->id);
	component->free();
	return ret;
}




void ECS::runStarts() {
	for(Component* i : startsToRun) {
		i->start();
		startsToRun.erase(i);
	}
}

void ECS::runLateUpdates(float delta) {
	for(auto i : hashes) {
		for(auto j : i.second) {
			j.second->lateUpdate(delta);
		}
	}
}

void ECS::runUpdates(float delta) {
	for(auto i : hashes) {
		for(auto j : i.second) {
			j.second->update(delta);
		}
	}
}


