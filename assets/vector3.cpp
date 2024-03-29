#include "vector3.h"
#include <assert.h>
#include <cmath>
#include <iostream>

float Vector3::rad2deg(float rad) {
	return 57.2957795131 * rad;
}

float Vector3::sqrmagnitude() {
	return x * x + y * y + z * z;
}
float Vector3::magnitude() {
	return sqrt(sqrmagnitude());
}

float Vector3::max() {
	return fmax(fmax(x, y), z);
}

float Vector3::min() {
	return fmin(fmin(x, y), z);
}


void Vector3::normalize(Vector3* output) {
	float magnitude = this->magnitude();
	output->x /= magnitude;
	output->y /= magnitude;
	output->z /= magnitude;
}
void Vector3::normalize() {
	this->normalize(this);
}

Vector3 Vector3::abs() {
	return new Vector3(fabs(this->x), fabs(this->y), fabs(this->z));
}
void Vector3::abs(Vector3* output) {
	output->x = fabs(this->x);
	output->y = fabs(this->y);
	output->z = fabs(this->z);
}


float Vector3::dot(Vector3* v2) {
	return this->x * v2->x + this->y * v2->y + this->z * v2->z;
}
float Vector3::dot(Vector3* v1, Vector3* v2) {
	return v1->dot(v2);
}


float Vector3::anglerad(Vector3* v2) {
	return acos(this->dot(v2) /
		(sqrt(this->sqrmagnitude() * v2->sqrmagnitude())));
}
float Vector3::angledeg(Vector3* v2) {
	return rad2deg(this->anglerad(v2));
}


void Vector3::project(Vector3* from, Vector3* onto, Vector3* output) {
	*output = *onto * (from->dot(onto) / pow(onto->magnitude(), 2));
}
void Vector3::project(Vector3* onto) {
	project(this, onto, this);
}


float Vector3::sqrdistance(Vector3* point2) {
	return pow(this->x - point2->x, 2) + pow(this->y - point2->y, 2) + pow(this->x - point2->x, 2);
}
float Vector3::distance(Vector3* point2) {
	return sqrt(this->sqrdistance(point2));
}


void Vector3::cross(Vector3* a, Vector3* b, Vector3* output) {
	// From https://www.mathworks.com/help/matlab/ref/cross.html
	output->x = a->y * b->z - a->z * b->y;
	output->y = a->z * b->x - a->x * b->z;
	output->z = a->x * b->y - a->y * b->x;
}
void Vector3::cross(Vector3* b) {
	cross(this, b, this);
}


void Vector3::lerp(Vector3* from, Vector3* to, float t, Vector3* output) {
	output->x = (to->x - from->x) * t + from->x;
	output->y = (to->y - from->y) * t + from->y;
    output->z = (to->z - from->z) * t + from->z;
}
void Vector3::lerp(Vector3* to, float t) {
	lerp(this, to, t, this);
}
void Vector3::lerpclamped(Vector3* from, Vector3* to, float t, Vector3* output) {
	lerp(from, to, fmax(fmin(t, 1), 0), output);
}
void Vector3::lerpclamped(Vector3* to, float t) {
	lerpclamped(this, to, t, this);
}

// this is messed up im too tired to fix it
void Vector3::orthogonal(Vector3* output) {
	//https://stackoverflow.com/questions/1171849/finding-quaternion-representing-the-rotation-from-one-vector-to-another#:~:text=cross(u%2C%20half))%3B%0A%7D-,The,-orthogonal%20function%20returns
	float x = std::abs(this->x);
	float y = std::abs(this->y);
	float z = std::abs(this->z);

	Vector3 other = (x < y ? (x < z ? Vector3Right : Vector3Up) : (y < z ? Vector3Backward : Vector3Up));
	Vector3::cross(this, &other, output);
}

void Vector3::cumMax(Vector3* a, Vector3* b, Vector3* output) {
	output->x = fmax(a->x, b->x);
	output->y = fmax(a->y, b->y);
	output->z = fmax(a->z, b->z);
}

void Vector3::cumMin(Vector3* a, Vector3* b, Vector3* output) {
	output->x = fmin(a->x, b->x);
	output->y = fmin(a->y, b->y);
	output->z = fmin(a->z, b->z);
}

void Vector3::print() {
	std::cout << "(" << this->x << ", " << this->y << ", " << this->z << ")" << std::endl;
}