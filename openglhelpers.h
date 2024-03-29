#ifndef OPENGLHELPERS_H
#define OPENGLHELPERS_H

#include "vector3.h"
#include "Quaternion.h"
#include "ecs.h"

class OpenGLHelpers {
    public:
        static void translateFromVector(Vector3* vec);
        static void scaleFromVector(Vector3* vec);
        static void rotateFromQuat(Quaternion* quat);
        static void applyTransformFields(TransformFields* t, int doScale, int doRotate, int doTranslate);
        static void applyTransformFields(TransformFields* t);
};

#endif