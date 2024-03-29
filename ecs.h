#ifndef ECS_H
#define ECS_H

#include <stdlib.h>
#include <math.h>
#include <string>
#include "vector3.h"
#include "Quaternion.h"
#include <unordered_map>
#include <typeinfo>
#include <iostream>
#include <type_traits>
#include <unordered_set>


using namespace std;

class Entity;

class Component {
	protected:
		int id_;
		int parent_id_;
		bool enabled_;
	public:
		Entity* entity;
		int getId() { return id; }
		bool getEnabled() { return enabled; }
		void setEnabled(bool enabled) { enabled_ = enabled; }
		virtual void awake(){}
		virtual void start(){}
		virtual void update(float delta){}
		virtual void lateUpdate(float delta){}
		virtual void free(){}
		virtual void kill();
		Component(int id, Entity* parentEntity);
		// https://stackoverflow.com/questions/5424042/class-variables-public-access-read-only-but-private-access-read-write
		// this is dumb, why am i still using this???
		const int& id = id_;
		const int& parent_id = parent_id_;
		const bool& enabled = enabled_;
};

struct TransformFields {
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
	void operator =(TransformFields* t) {
		position = t->position;
		rotation = t->rotation;
		scale = t->scale;
	}
	void operator =(TransformFields t) {
		*this = &t;
	}
};

class Transform : public Component {
	public:
		virtual void awake();
		struct Vector3 position;
		struct Quaternion rotation;
		struct Vector3 scale;
		Transform(int id, Entity* parent):Component(id, parent) {};
		void hierarchyScale(Vector3* output);
		void hierarchyRotation(Quaternion* output);
		void hierarchyPosition(Vector3* output);
		void hierarchyTransform(TransformFields* fields);
		bool relative;
};

class ECS;

class Entity : public Component {
	public:
		Transform* transform;
		int layer;
		std::string name;
		Entity(int id, Entity* parent):Component(id, parent) {};
		template <class T> T* getComponent();
		template <class T> T* addComponent();
		template <class T> bool removeComponent();
};

class ECS {
	private:
		static int component_id_counter;
		static Entity* createEntity(int id, Entity* parent);
		ECS();
	public:
		static std::unordered_map<std::size_t, std::unordered_map<int, Component*>> hashes;
		static unordered_set<Component*> startsToRun;
		// unordered_map<typeid(component type).name, unordered_map<component.id, pointer to component>>
		static Entity* instantiate();
		static Entity* getEntityById(int id);
		static bool kill(Component* component);
		static void runUpdates(float delta);
		static void runLateUpdates(float delta);
		static void runStarts();
		template <class T> static T* getComponentByParent(Entity* parent)
		{
			if(hashes.find(typeid(T*).hash_code()) != hashes.end()) {
				auto returnpair = (hashes[typeid(T*).hash_code()].find(parent->id));
				if(returnpair != hashes[typeid(T*).hash_code()].end()) {
					return dynamic_cast<T*>(returnpair->second);
				}
				return NULL;
			}
			return NULL;
		}
		template <class T> static T* createComponent(Entity* parent)
		{
			if constexpr(std::is_base_of_v<Component, T>) {
				T* ret = (T*)malloc(sizeof(T));
				ret = new (ret)T(component_id_counter, parent);
				component_id_counter++;
				hashes[typeid(T*).hash_code()][parent->id] = dynamic_cast<Component*>(ret);
				startsToRun.insert(dynamic_cast<Component*>(ret));
				ret->awake();
				return ret;
			}
			else {
				return NULL;
			}
		}
		static bool destroyComponent(Component* component);
		template <class T> static std::unordered_map<int, Component*>* getAllComponentInstances()
		{
			return &hashes[typeid(T).hash_code()];
		}
};

template <class T> T* Entity::getComponent()
{
	return ECS::getComponentByParent<T>(this);
}

template <class T> T* Entity::addComponent()
{
		T* checkIfExists = getComponent<T>();
		if(checkIfExists == NULL) {
			return ECS::createComponent<T>(this);
		} else {
			printf("Tried to add a component that already exists.");
			return checkIfExists;
		}
}

template <class T> bool Entity::removeComponent()
{
	T* checkIfExists = getComponent<T>();
	if(checkIfExists == NULL) {
		return false; }
	else {
		ECS::destroyComponent(checkIfExists);
		return true;
	}
}


#endif