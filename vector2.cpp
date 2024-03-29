#include "vector2.h"
#include "macros.h"
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

void Vector2::print() {
	printf("(%.6f, %.6f) \n", x, y);
}

float Vector2::sqrmagnitude() {
	return x*x + y*y;
}

float Vector2::magnitude() {
	return sqrt(sqrmagnitude());
}