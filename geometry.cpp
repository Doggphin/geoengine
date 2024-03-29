#include "geometry.h"
#include "macros.h"
#include <cmath>
#include <limits>

Vector3 Geometry::applyReverseTransform(Vector3* point, TransformFields* t, int doRotation) {
	struct Vector3 ret;
	ret = point;
	ret -= t->position;
	if(doRotation) {
		struct Quaternion conjugate;
		Quaternion::conjugate(&t->rotation, &conjugate);
		Quaternion::rotate(&conjugate, &ret, &ret);
	}
	ret /= t->scale;
	return ret;
}
Vector3 Geometry::applyTransform(Vector3* point, TransformFields* t, int doRotation) {
	struct Vector3 ret;
	ret = point;
	ret *= t->scale;
	if(doRotation) {
		Quaternion::rotate(&t->rotation, &ret, &ret);
	}
	ret += t->position;
	return ret;
}


void Geometry::generateAABB(std::list<Vector3*>* points, AABB* out) {
	struct Vector3 min = points->front();
	struct Vector3 max = points->front();
	for(auto i : *points) {
		min = Vector3(fmin(i->x, min.x), fmin(i->y, min.y), fmin(i->z, min.z));
		max = Vector3(fmax(i->x, max.x), fmax(i->y, max.y), fmax(i->z, max.z));
	}
	out->max = max;
	out->min = min;
}
void Geometry::generateAABB(Vector3* center, Vector3* extents, AABB* out) {
	assert(out != NULL);

	out->max = Vector3(center->x + extents->x / 2, center->y + extents->y / 2, center->z + extents->z / 2);
	out->min = Vector3(center->x - extents->x / 2, center->y - extents->y / 2, center->z - extents->z / 2);
}


int Geometry::isWithinBounds(Vector3* min, Vector3* max, Vector3* point) {
	return 	point->x > min->x &&
			point->y > min->y &&
			point->z > min->z &&
			point->x < max->x &&
			point->y < max->y &&
			point->z < max->z;
}

// https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection
int Geometry::AABBIsInAABB(AABB* a, AABB* b) {
	return a->min.x <= b->max.x &&
	a->max.x >= b->min.x &&
	a->min.y <= b->max.y &&
	a->max.y >= b->min.y &&
	a->min.z <= b->max.z &&
	a->max.z >= b->min.z;
}


void Geometry::extentsToPoints(Vector3* min, Vector3* max, std::list<Vector3*>* output) {
	*output = {
		new Vector3(min->x, min->y, min->z),
		new Vector3(min->x, min->y, max->z),
		new Vector3(min->x, max->y, min->z),
		new Vector3(min->x, max->y, max->z),
		new Vector3(max->x, min->y, min->z),
		new Vector3(max->x, min->y, max->z),
		new Vector3(max->x, max->y, min->z),
		new Vector3(max->x, max->y, max->z)
	};
}

void Geometry::applyTransformToAABB(AABB* a, TransformFields* t, AABB* output) {
	// This is REMARKABLY inefficient for something that doesnt
	// even give a good result. PLEASE FOR THE LOVE OF GOD OPTIMIZE ME

	// perhaps check how aligned the transformfields are with the world axes.
	// then multiply AABB by (0-1 of how not aligned it is * 1/sqrt(1+1+1))

	// finally, this should just be the last resort and NOT what is done normally.
	// For example, against a capsule, this is much easier.
	std::list<Vector3*> testList;
	Geometry::extentsToPoints(&a->min, &a->max, &testList);
	struct Vector3 temp;
	struct Vector3 min = a->min;
	struct Vector3 max = a->max;
	for(auto i: testList) {
		Quaternion::rotate(&t->rotation, i, &temp);
		min = Vector3(fmin(temp.x, min.x), fmin(temp.y, min.y), fmin(temp.z, min.z));
		max = Vector3(fmax(temp.x, max.x), fmax(temp.y, max.y), fmax(temp.z, max.z));
	}
	min *= t->scale;
	max *= t->scale;
	output->min = min + t->position;
	output->max = max + t->position;
}


void EmptyShape::generateAABB(AABB* output) {
	output = new AABB(new Vector3(0,0,0), new Vector3(0,0,0));
}
void EmptyShape::generateAABB(TransformFields* t, AABB* output) {
	generateAABB(output);
}
int EmptyShape::containsPoint(Vector3* point) {
	return 0;
}
int EmptyShape::containsPoint(Vector3* point, TransformFields* t) {
	return 0;
}
Vector3 EmptyShape::closestPoint(Vector3* point) {
	return Vector3(0,0,0);
}
Vector3 EmptyShape::closestPoint(Vector3* point, TransformFields* t) {
	struct Vector3 newPoint = Geometry::applyReverseTransform(point, t, 0);
	struct Vector3 closest = closestPoint(&newPoint);
	return Geometry::applyTransform(&closest, t, 0);
}

void Plane::generateAABB(AABB* output) {
	output->max = new Vector3(normal.x == 1 ? 0 : std::numeric_limits<float>::infinity(),
	normal.y == 1 ? 0 : std::numeric_limits<float>::infinity(),
	normal.z == 1 ? 0 : std::numeric_limits<float>::infinity());
	output->min = output->max * -1;
}
void Plane::generateAABB(TransformFields* t, AABB* output) {
	struct Vector3 newNormal; Quaternion::rotate(&t->rotation, &normal, &newNormal);
	output->max = new Vector3(newNormal.x == 1 ? 0 : std::numeric_limits<float>::infinity(),
	newNormal.y == 1 ? 0 : std::numeric_limits<float>::infinity(),
	newNormal.z == 1 ? 0 : std::numeric_limits<float>::infinity());
	output->min = output->max * -1;
}
// Planes can't contain a point
int Plane::containsPoint(Vector3* point) {
	return 0;
}
int Plane::containsPoint(Vector3* point, TransformFields* t) {
	return 0;
}
//https://mathinsight.org/distance_point_plane
Vector3 Plane::closestPoint(Vector3* point) {
	float distance = (normal.x * point->x + normal.y * point->y + normal.z * point->z);
	return *point - (normal * distance);
}
Vector3 Plane::closestPoint(Vector3* point, TransformFields* t) {
	struct Vector3 newPoint = Geometry::applyReverseTransform(point, t, 1);
	struct Vector3 closest = closestPoint(&newPoint);
	return Geometry::applyTransform(&closest, t, 1);
}

void FinitePlane::generateAABB(AABB* output) {
	output->max = new Vector3(width * .5, 0, length * .5);
	output->min = output->max * -1;
}
void FinitePlane::generateAABB(TransformFields* t, AABB* output) {
	struct Vector3 newNormal; Quaternion::rotate(&t->rotation, &normal, &newNormal);
	output->max = new Vector3(newNormal.x == 1 ? 0 : std::numeric_limits<float>::infinity(),
	newNormal.y == 1 ? 0 : std::numeric_limits<float>::infinity(),
	newNormal.z == 1 ? 0 : std::numeric_limits<float>::infinity());
	output->min = output->max * -1;
}
// Planes can't contain a point
int FinitePlane::containsPoint(Vector3* point) {
	return 0;
}
int FinitePlane::containsPoint(Vector3* point, TransformFields* t) {
	return 0;
}
//https://mathinsight.org/distance_point_plane
Vector3 FinitePlane::closestPoint(Vector3* point) {
	float distance = (normal.x * point->x + normal.y * point->y + normal.z * point->z);
	return *point - (normal * distance);
}
Vector3 FinitePlane::closestPoint(Vector3* point, TransformFields* t) {
	struct Vector3 newPoint = Geometry::applyReverseTransform(point, t, 1);
	struct Vector3 closest = closestPoint(&newPoint);
	return Geometry::applyTransform(&closest, t, 1);
}


void AABB::generateAABB(AABB* output) {
	output->max = max;
	output->min = min;
}
void AABB::generateAABB(TransformFields* t, AABB* output) {
	output->max = (max * t->scale) + t->position;
	output->min = (min * t->scale) + t->position;
}
int AABB::containsPoint(Vector3* point) {
	return Geometry::isWithinBounds(&min, &max, point);
}
int AABB::containsPoint(Vector3* point, TransformFields* t) {
	struct Vector3 newPoint = Geometry::applyReverseTransform(point, t, 0);
	return containsPoint(&newPoint);
}
Vector3 AABB::closestPoint(Vector3* point) {
	return new Vector3(
		point->x > max.x ? max.x :
		point->x < min.x ? min.x :
		point->x,

		point->y > max.y ? max.y :
		point->y < min.y ? min.y :
		point->y,

		point->z > max.z ? max.z :
		point->z < min.z ? min.z :
		point->z
	);
}
Vector3 AABB::closestPoint(Vector3* point, TransformFields* t) {
	struct Vector3 newPoint = Geometry::applyReverseTransform(point, t, 0);
	struct Vector3 closest = closestPoint(&newPoint);
	return Geometry::applyTransform(&closest, t, 0);
}


void Sphere::generateAABB(AABB* output) {
	struct Vector3 newScale = new Vector3(radius, radius, radius);
	Geometry::generateAABB(new Vector3(0,0,0), &newScale, output);
}
void Sphere::generateAABB(TransformFields* t, AABB* output) {
	float maxscale = t->scale.max();
	output->min = t->position - (radius * maxscale);
	output->max = t->position + (radius * maxscale);
}
int Sphere::containsPoint(Vector3* point) {
	return point->sqrmagnitude() < radius * radius;
}
int Sphere::containsPoint(Vector3* point, TransformFields* t) {
	struct Vector3 newPoint = Geometry::applyReverseTransform(point, t, 0);
	return containsPoint(&newPoint);
}
Vector3 Sphere::closestPoint(Vector3* point) {
	float sqrmagnitude = point->sqrmagnitude();
	if(sqrmagnitude < radius * radius) {
		return new Vector3(point->x, point->y, point->z);
	} else {
		return *point * ((1/sqrmagnitude) * radius);
	}
}
Vector3 Sphere::closestPoint(Vector3* point, TransformFields* t) {
	struct Vector3 newPoint = Geometry::applyReverseTransform(point, t, 0);
	struct Vector3 closest = closestPoint(&newPoint);
	return Geometry::applyTransform(&closest, t, 0);
}


void Capsule::generateAABB(AABB* output) {
	struct Vector3 topEnd = new Vector3(radius, height + radius, radius);
	struct Vector3 bottomEnd = topEnd * -1;
	std::list<Vector3*> testList = {&topEnd, &bottomEnd};
	Geometry::generateAABB(&testList, output);
}
Vector3 Capsule::getTop() {
	struct Vector3 ret = (Vector3{direction == DIRECTION_LEFT || direction == DIRECTION_RIGHT ? .5f * height : 0,
						 			direction == DIRECTION_UP || direction == DIRECTION_DOWN ? .5f * height : 0,
									direction == DIRECTION_FORWARD || direction == DIRECTION_BACKWARD ? .5f * height : 0,
									});
	return ret;
}
void Capsule::getAxis(LineSegment* output) {
	struct Vector3 start = getTop();
	struct Vector3 end = start * -1;
	output = new LineSegment(&start, &end);
}
void Capsule::generateAABB(TransformFields* t, AABB* output) {
	// AABB does not seem to have the radius added to it
	float maxscale = t->scale.max();
	struct Vector3 topEnd = getTop() * maxscale;
	struct Vector3 topRotated; Quaternion::rotate(&t->rotation, &topEnd, &topRotated);
	struct Vector3 botRotated = topRotated * -1;
	Vector3::cumMax(&botRotated, &topRotated, &output->max);
	Vector3::cumMin(&botRotated, &topRotated, &output->min);
	output->max += (maxscale * radius);
	output->min += -(maxscale * radius);
	output->max += t->position;
	output->min += t->position;
}
int Capsule::containsPoint(Vector3* point) {
	struct LineSegment line;
	getAxis(&line);
	return Geometry::sqrDistanceToLineSegment(point, &line) < (radius * radius);
}
int Capsule::containsPoint(Vector3* point, TransformFields* t) {
	struct Vector3 newPoint = Geometry::applyReverseTransform(point, t, 1);
	return containsPoint(&newPoint);
}
Vector3 Capsule::closestPoint(Vector3* point) {
	struct Vector3 ret;
	struct LineSegment line; getAxis(&line);
	struct Vector3 toCapsule; Geometry::vectorToLineSegment(point, &line, &toCapsule);
	if(toCapsule.sqrmagnitude() < (radius * radius)) {
		ret = point;
	}
	else {
		struct Vector3 pointOnAxis = toCapsule + point;
		// Make toCapsule represent line pointing out of capsule axis towards the point
		toCapsule.normalize();
		toCapsule *= -1;
		toCapsule *= radius;
		ret = pointOnAxis + toCapsule;
	}
	return ret;
}
Vector3 Capsule::closestPoint(Vector3* point, TransformFields* t) {
	struct Vector3 newPoint = Geometry::applyReverseTransform(point, t, 1);
	struct Vector3 closest = closestPoint(&newPoint);
	return Geometry::applyTransform(&closest, t, 1);
}

void Cuboid::generateAABB(AABB* output) {
	output->max = scale * .5;
	output->min = scale * -.5;
}
void Cuboid::generateAABB(TransformFields* t, AABB* output) {
	generateAABB(output);
	Geometry::applyTransformToAABB(output, t, output);
}
int Cuboid::containsPoint(Vector3* point) {
	struct Vector3 min = scale * -.5;
	struct Vector3 max = scale * .5;
	return Geometry::isWithinBounds(&min, &max, point);
}
int Cuboid::containsPoint(Vector3* point, TransformFields* t) {
	struct Vector3 newPoint = Geometry::applyReverseTransform(point, t, 1);
	return containsPoint(&newPoint);
}
Vector3 Cuboid::closestPoint(Vector3* point) {
	struct Vector3 min = scale * -.5;
	struct Vector3 max = scale * .5;
	return new Vector3(
		point->x > max.x ? max.x :
		point->x < min.x ? min.x :
		point->x,

		point->y > max.y ? max.y :
		point->y < min.y ? min.y :
		point->y,

		point->z > max.z ? max.z :
		point->z < min.z ? min.z :
		point->z
	);
}
Vector3 Cuboid::closestPoint(Vector3* point, TransformFields* t) {
	struct Vector3 newPoint = Geometry::applyReverseTransform(point, t, 1);
	struct Vector3 closest = closestPoint(&newPoint);
	return Geometry::applyTransform(&closest, t, 1);
}

// https://www.geeksforgeeks.org/minimum-distance-from-a-point-to-the-line-segment-using-vectors/
void Geometry::vectorToLineSegment(Vector3* point, LineSegment* segment, Vector3* output) {
	struct Vector3 AB = segment->start - segment->end;
	struct Vector3 AP = *point - segment->start;
	struct Vector3 BP = *point - segment->end;
	float AB_BP = AB.dot(&BP);
	float AB_AP = AB.dot(&AP);
	if(AB_BP > 0) {
		*output = *point - segment->end;
	}
	if(AB_AP < 0) {
		*output = *point - segment->start;
	}
	else {
		struct Vector3 ret;
		Vector3::cross(&segment->start, &segment->end, &ret);
	}
}

float Geometry::sqrDistanceToLineSegment(Vector3* point, LineSegment* segment) {
	struct Vector3 temp;
	vectorToLineSegment(point, segment, &temp);
	return temp.sqrmagnitude();
}

float Geometry::distanceToLineSegment(Vector3* point, LineSegment* segment) {
	return sqrt(sqrDistanceToLineSegment(point, segment));
}

char Geometry::computeCohenSutherlandCode(Vector3* min, Vector3* max, Vector3* point) {
	char out = 0;

	if(point->x < min->x) {
		out |= 1;
	} else if (point->x > max->x) {
		out |= 2;
	}

	if(point->y < min->y) {
		out |= 4;
	} else if (point->y > max->y) {
		out |= 8;
	}

	if(point->z < min->z) {
		out |= 16;
	} else if (point->z > max->z) {
		out |= 32;
	}

	return out;
}

// https://www.mdpi.com/1999-4893/16/4/201
bool Geometry::clipLine(Vector3* min, Vector3* max, Vector3* point1, Vector3* point2, Vector3* point1Out, Vector3* point2Out, char* point1OutCode, char* point2OutCode, bool allowInside, bool allowStartInside) {
	*point1OutCode = Geometry::computeCohenSutherlandCode(min, max, point1);
	*point2OutCode = Geometry::computeCohenSutherlandCode(min, max, point2);
	if( (*point1OutCode == 0 && !allowStartInside) || (!(*point1OutCode | *point2OutCode)) || ((*point1OutCode & *point2OutCode)) ) {
		return false;
	}	
	float a = point2->x - point1->x;
	float b = point2->y - point1->y;
	float c = point2->z - point1->z;
	*point1Out = point1;
	*point2Out = point2;
	for(int i=0; i<=1; i++) {
		Vector3* pointer = i == 0 ? point1Out : point2Out;
		char* codePointer = i == 0 ? point1OutCode : point2OutCode;
		if(pointer->x < min->x) {
			pointer->y = b / a * (min->x - point1->x) + point1->y;
			pointer->z = c / a * (min->x - point1->x) + point1->z;
			pointer->x = min->x;
			*codePointer = 1;
		}
		else if(pointer->x > max->x) {
			pointer->y = b / a * (max->x - point1->x) + point1->y;
			pointer->z = c / a * (max->x - point1->x) + point1->z;
			pointer->x = max->x;
			*codePointer = 2;
		}
		if(pointer->y < min->y) {
			pointer->x = a / b * (min->y - point1->y) + point1->x;
			pointer->z = c / b * (min->y - point1->y) + point1->z;
			pointer->y = min->y;
			*codePointer = 4;
		}
		else if(pointer->y > max->y) {
			pointer->x = a / b * (max->y - point1->y) + point1->x;
			pointer->z = c / b * (max->y - point1->y) + point1->z;
			pointer->y = max->y;
			*codePointer = 8;
		}
		if(pointer->z < min->z) {
			pointer->x = a / c * (min->z - point1->z) + point1->x;
			pointer->y = b / c * (min->z - point1->z) + point1->y;
			pointer->z = min->z;
			*codePointer = 16;
		}
		else if(pointer->z > max->z) {
			pointer->x = a / c * (max->z - point1->z) + point1->x;
			pointer->y = b / c * (max->z - point1->z) + point1->y;
			pointer->z = max->z;
			*codePointer = 32;
		}
	}
	if(*point1Out == *point2Out) { return false; }
	return true;	
}

int Geometry::lineIntersectsAABB(AABB* aabb, Vector3* lineStart, Vector3* lineEnd) {
	return(!(Geometry::computeCohenSutherlandCode(&aabb->min, &aabb->max, lineStart) |  Geometry::computeCohenSutherlandCode(&aabb->min, &aabb->max, lineEnd)));
}
int Geometry::rayIntersectsAABB(AABB* aabb, Vector3* lineStart, Vector3* lineDirection, float t) {
	struct Vector3 lineEnd = *lineStart + (*lineDirection * t);
	return(Geometry::lineIntersectsAABB(aabb, lineStart, &lineEnd));
}

// Expands the AABB by the radius of the circle. Works everywhere except the corners of the AABB as it won't be rounded, but this is faster
int Geometry::approxCapsuleIntersectsAABB(AABB* aabb, Vector3* lineStart, Vector3* lineEnd, float radius ) {
	struct Vector3 newMin = (aabb->min - radius);
	struct Vector3 newMax = (aabb->max + radius);
	return(!(Geometry::computeCohenSutherlandCode(&newMin, &newMax, lineStart) |  Geometry::computeCohenSutherlandCode(&newMin, &newMax, lineEnd)));
}
int Geometry::approxSpherecastIntersectsAABB(AABB* aabb, Vector3* lineStart, Vector3* lineDirection, float t, float radius ) {
	struct Vector3 lineEnd = *lineStart + (*lineDirection * t);
	return(Geometry::approxCapsuleIntersectsAABB(aabb, lineStart, &lineEnd, radius));
}
