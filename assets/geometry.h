#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "vector3.h"
#include "ecs.h"
#include "Quaternion.h"
#include <assert.h>
#include <vector>
#include <list>

typedef enum PrimitiveShape {
	SHAPE_NONE, SHAPE_AABB, SHAPE_SPHERE, SHAPE_CUBE, SHAPE_CAPSULE
} PrimitiveShape;

typedef enum Direction {
	DIRECTION_UP, DIRECTION_DOWN, DIRECTION_RIGHT, DIRECTION_LEFT, DIRECTION_FORWARD, DIRECTION_BACKWARD
} Direction;

typedef struct ShapeBase ShapeBase;
typedef struct AABB AABB;

struct LineSegment {
	struct Vector3 start;
	struct Vector3 end;
	LineSegment() {
		this->start = Vector3Zero;
		this->end = Vector3Zero;
	}
	LineSegment(Vector3* start, Vector3* end) {
		this->start = start;
		this->end = end;
	}
};

struct ShapeBase {
	virtual void generateAABB(AABB* output) = 0;
	virtual void generateAABB(TransformFields* t, AABB* output) = 0;
	virtual int containsPoint(Vector3* point) = 0;
	virtual int containsPoint(Vector3* point, TransformFields* t) = 0;
	virtual Vector3 closestPoint(Vector3* point) = 0;
	virtual Vector3 closestPoint(Vector3* point, TransformFields* t) = 0;
};

#define SHAPE_INHERIT_FUNCTIONS\
	virtual void generateAABB(AABB* output); \
	virtual void generateAABB(TransformFields* t, AABB* output); \
	virtual int containsPoint(Vector3* point); \
	virtual int containsPoint(Vector3* point, TransformFields* t); \
	virtual Vector3 closestPoint(Vector3* point); \
	virtual Vector3 closestPoint(Vector3* point, TransformFields* t);

struct Plane : public ShapeBase {
	struct Vector3 normal;

	SHAPE_INHERIT_FUNCTIONS
};

struct FinitePlane : public ShapeBase {
	struct Vector3 normal;
	float width;
	float length;

	SHAPE_INHERIT_FUNCTIONS
};

// Axis-Aligned Bounding Box. Does not consider rotations.
struct AABB : public ShapeBase {
	struct Vector3 max;
	struct Vector3 min;
	AABB(Vector3* max, Vector3* min) {
		this->max = max;
		this->min = min;
	}

	SHAPE_INHERIT_FUNCTIONS
};

struct EmptyShape : public ShapeBase {
	SHAPE_INHERIT_FUNCTIONS
};

struct Capsule : public ShapeBase {
	float height;
	float radius;
	Direction direction;\
	Capsule(float height, float radius) {
		this->height = height;
		this->radius = radius;
		direction = DIRECTION_UP;
	}
	Vector3 getTop();
	void getAxis(LineSegment* output);

	SHAPE_INHERIT_FUNCTIONS
};

struct Sphere : public ShapeBase {
	float radius;
	Sphere(float radius) {
		this->radius = radius;
	}

	SHAPE_INHERIT_FUNCTIONS
};

// A rotated axis aligned bounding box.
struct Cuboid : public ShapeBase {
	struct Vector3 scale;
	Cuboid(Vector3* scale) {
		this->scale = scale;
	}

	SHAPE_INHERIT_FUNCTIONS
};

struct MeshShape : public ShapeBase {
	std::vector<Vector3> vertices;
};

class Geometry {
	public:
		//static void generateAABBShape(Vector3* center, void* shape, PrimitiveShape shapeType, AABB* out);

		static void extentsToPoints(Vector3* min, Vector3* max, std::list<Vector3*>* output);

		static void generateAABB(std::list<Vector3*>* points, AABB* out);
		static void generateAABB(Vector3* center, Vector3* extents, AABB* out);
		static void applyTransformToAABB(AABB* a, TransformFields* t, AABB* output);
		static void vectorToLineSegment(Vector3* point, LineSegment* segment, Vector3* output);
		static float sqrDistanceToLineSegment(Vector3* point, LineSegment* segment);
		static float distanceToLineSegment(Vector3* point, LineSegment* segment);

		static char computeCohenSutherlandCode(Vector3* min, Vector3* max, Vector3* point);
		static bool clipLine(Vector3* min, Vector3* max, Vector3* point1, Vector3* point2, Vector3* point1Out, Vector3* point2Out, char* point1OutCode, char* point2OutCode, bool allowFullInside, bool allowStartInside);

		static int AABBIsInAABB(AABB* a, AABB* b);
		static int isWithinBounds(Vector3* min, Vector3* max, Vector3* point);

		static int lineIntersectsAABB(AABB* aabb, Vector3* lineStart, Vector3* lineEnd);
		static int rayIntersectsAABB(AABB* aabb, Vector3* lineStart, Vector3* lineDirection, float t);
		static int approxCapsuleIntersectsAABB(AABB* aabb, Vector3* lineStart, Vector3* lineEnd, float radius);
		static int approxSpherecastIntersectsAABB(AABB* aabb, Vector3* lineStart, Vector3* lineDirection, float t, float radius);
		static Vector3 applyReverseTransform(Vector3* point, TransformFields* t, int doRotation);
		static Vector3 applyTransform(Vector3* point, TransformFields* t, int doRotation);
};


#endif