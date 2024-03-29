#include "gizmodrawer.h"
#include "openglhelpers.h"
#include <math.h>
#include "vector2.h"

Vector2 GizmoDrawer::circlePoints[16] = {
    {0, 1},
    {.3827, .9239},
    {.7071, .7071},
    {.9239, .3827},
    {1, 0},
    {.9239, -.3827},
    {.7071, -.7071},
    {.3827, -.9239},
    {0, -1},
    {-.3827, -.9239},
    {-.7071, -.7071},
    {-.9239, -.3827},
    {-1, 0},
    {-.9239, .3827},
    {-.7071, .7071},
    {-.3827, .9239},
};

void GizmoDrawer::drawSphere(float radius) {
    glBegin(GL_LINE_STRIP);
    for(int i=0; i<16; i++) {
        glVertex3d(circlePoints[i].x * radius, circlePoints[i].y * radius, 0);
    }
    glVertex3d(0, radius, 0);
    glEnd();

    glBegin(GL_LINE_STRIP);
    for(int i=0; i<16; i++) {
        glVertex3d(0, circlePoints[i].x * radius, circlePoints[i].y * radius);
    }
    glVertex3d(0, 0, radius);
    glEnd();

    glBegin(GL_LINE_STRIP);
    for(int i=0; i<16; i++) {
        glVertex3d(circlePoints[i].x * radius, 0, circlePoints[i].y * radius);
    }
    glVertex3d(0, 0, radius);
    glEnd();
}

void GizmoDrawer::drawCapsule(float radius, float height) {
    height *= .5;
    glBegin(GL_LINE_STRIP);
    for(int i=0; i<5; i++) {
        glVertex3d(circlePoints[i].x * radius, circlePoints[i].y * radius + height, 0);
    }
    for(int i=4; i<13; i++) {
        glVertex3d(circlePoints[i].x * radius, circlePoints[i].y * radius - height, 0);
    }
    for(int i=12; i<16; i++) {
        glVertex3d(circlePoints[i].x * radius, circlePoints[i].y * radius + height, 0);
    }
    glVertex3d(0, radius + height, 0);
    glEnd();

    glBegin(GL_LINE_STRIP);
    for(int i=0; i<5; i++) {
        glVertex3d(0, circlePoints[i].y * radius + height, circlePoints[i].x * radius);
    }
    for(int i=4; i<13; i++) {
        glVertex3d(0, circlePoints[i].y * radius - height, circlePoints[i].x * radius);
    }
    for(int i=12; i<16; i++) {
        glVertex3d(0, circlePoints[i].y * radius + height, circlePoints[i].x * radius);
    }
    glVertex3d(0, radius + height, 0);
    glEnd();

    glBegin(GL_LINE_STRIP);
    for(int i=0; i<16; i++) {
        glVertex3d(circlePoints[i].x * radius, height, circlePoints[i].y * radius);
    }
    glVertex3d(0, height, radius);
    glEnd();

    glBegin(GL_LINE_STRIP);
    for(int i=0; i<16; i++) {
        glVertex3d(circlePoints[i].x * radius, -height, circlePoints[i].y * radius);
    }
    glVertex3d(0, -height, radius);
    glEnd();
}


void GizmoDrawer::drawMinMax(Vector3* min, Vector3* max) {
    glBegin(GL_LINE_STRIP);
	glVertex3d(min->x, min->y, min->z);
	glVertex3d(max->x, min->y, min->z);
	glVertex3d(max->x, max->y, min->z);
	glVertex3d(min->x, max->y, min->z);
	glVertex3d(min->x, min->y, min->z);
	glVertex3d(min->x, min->y, max->z);
	glVertex3d(min->x, max->y, max->z);
	glVertex3d(min->x, max->y, min->z);
	glEnd();
	glBegin(GL_LINES);
	glVertex3d(max->x, max->y, max->z);
	glVertex3d(min->x, max->y, max->z);
	glVertex3d(max->x, max->y, max->z);
	glVertex3d(max->x, min->y, max->z);
	glVertex3d(max->x, max->y, max->z);
	glVertex3d(max->x, max->y, min->z);
	glVertex3d(max->x, min->y, max->z);
	glVertex3d(max->x, min->y, min->z);
	glVertex3d(max->x, min->y, max->z);
	glVertex3d(min->x, min->y, max->z);
	glEnd();
}

void GizmoDrawer::drawAABB(AABB* aabb) {
    GizmoDrawer::drawMinMax(&aabb->min, &aabb->max);
}

void GizmoDrawer::drawCube(Vector3* scale) {
    struct Vector3 max = new Vector3(scale->x * .5, scale->y * .5, scale->z * .5);
    struct Vector3 min = max * -1;
    GizmoDrawer::drawMinMax(&min, &max);
}

void GizmoDrawer::drawCollider(Collider* collider) {
    if(collider->drawGizmo == 0) { return; }

    struct TransformFields t;
    collider->getTransformFields(&t);

    glPushMatrix();
    //OpenGLHelpers::translateFromVector(&t.position);
    switch(collider->colliderShapeType) {

        case SHAPE_SPHERE: {
            OpenGLHelpers::applyTransformFields(&t, 1, 0, 1);
            drawSphere(static_cast<Sphere*>(collider->colliderShape)->radius);
        } break;

        case SHAPE_CAPSULE: {
            OpenGLHelpers::applyTransformFields(&t, 1, 1, 1);
            Capsule* capsule = static_cast<Capsule*>(collider->colliderShape);
            if(capsule->direction == DIRECTION_LEFT || capsule->direction == DIRECTION_RIGHT) { glRotatef(90, 0, 0, 1); }
            else if(capsule->direction == DIRECTION_BACKWARD || capsule->direction == DIRECTION_FORWARD) { glRotatef(90, 1, 0, 0); }
            drawCapsule(capsule->radius, capsule->height);
        } break;

        case SHAPE_CUBE: {
            OpenGLHelpers::applyTransformFields(&t, 1, 1, 1);
            drawCube(&(static_cast<Cuboid*>(collider->colliderShape)->scale));
        } break;

        case SHAPE_AABB: {
            OpenGLHelpers::applyTransformFields(&t, 1, 0, 1);
            AABB* aabb = static_cast<AABB*>(collider->colliderShape);
            drawAABB(aabb);
        } break;

        default: {
            
        } break;
    }
    glPopMatrix();
}