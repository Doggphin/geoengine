#include "openglhelpers.h"
#include "CSCIx229.h"

void OpenGLHelpers::translateFromVector(Vector3* vec) {
	glTranslatef(vec->x, vec->y, vec->z);
}

void OpenGLHelpers::scaleFromVector(Vector3* vec) {
	glScalef(vec->x, vec->y, vec->z);
}

void OpenGLHelpers::rotateFromQuat(Quaternion* quat) {
	float* qm = quat->toMat4();
	GLdouble rotMatrix[16] = {qm[0], qm[1], qm[2], qm[3], qm[4], qm[5], qm[6], qm[7], qm[8], qm[9], qm[10], qm[11], qm[12], qm[13], qm[14], qm[15]};
	glMultMatrixd(rotMatrix);
	free(qm);
}

void OpenGLHelpers::applyTransformFields(TransformFields* t, int doScale, int doRotate, int doTranslate) {
	if(doTranslate) { translateFromVector(&t->position); }
	if(doScale) { scaleFromVector(&t->scale); }
	if(doRotate) { rotateFromQuat(&t->rotation); }
}

void OpenGLHelpers::applyTransformFields(TransformFields* t) {
	OpenGLHelpers::applyTransformFields(t, 1, 1, 1);
}