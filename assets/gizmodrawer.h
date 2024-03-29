#ifndef GIZMODRAWER_H
#define GIZMODRAWER_H
#include "CSCIx229.h"
#include "comp_collider.h"
#include "vector2.h"

class GizmoDrawer {
    private:
        static Vector2 circlePoints[16];
    public:
        static void drawCollider(Collider* collider);
        static void drawMinMax(Vector3* min, Vector3* max);
        static void drawAABB(AABB* aabb);
        static void drawSphere(float radius);
        static void drawCapsule(float radius, float height);
        static void drawCube(Vector3* scale);
};

#endif