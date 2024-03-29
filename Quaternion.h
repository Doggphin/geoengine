// Copyright (C) 2019 Martin Weigel <mail@MartinWeigel.com>
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

/**
 * @file    Quaternion.h
 * @brief   A basic quaternion library written in C
 * @date    2019-11-28
 */
#pragma once
#include "vector3.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

/**
 * Maximum floating point difference that is considered as equal.
 */
#define QUATERNION_EPS (1e-4)

/**
 * Data structure to hold a quaternion.
 */
typedef struct Quaternion {
    float w;       /**< Scalar part */
    float x;
    float y;
    float z;    /**< Vector part */
    void operator =(Quaternion* a) {
        w = a->w; x = a->x; y = a->y; z = a->z;
    }
    void operator =(Quaternion a) {
        *this = &a;
    }
    Quaternion operator *(Quaternion* q2) {
        return Quaternion(w  * q2->w       - x*q2->x - y*q2->y - z*q2->z, x  * q2->w       + w*q2->x + y*q2->z - z*q2->y, w  * q2->y       - x*q2->z + y*q2->w + z*q2->x, w  * q2->z       + x*q2->y - y*q2->x + z*q2->w);
    }
    void operator *=(Quaternion* q2) {
        Quaternion result;
        result.w = w  * q2->w       - x*q2->x - y*q2->y - z*q2->z;
        result.x = x  * q2->w       + w*q2->x + y*q2->z - z*q2->y;
        result.y = w  * q2->y       - x*q2->z + y*q2->w + z*q2->x;
        result.z = w  * q2->z       + x*q2->y - y*q2->x + z*q2->w;
        *this = &result;
    }
    void operator *=(Quaternion q2) {
        *this *= &q2;
    }

    Quaternion(float w, float x, float y, float z) {
        this->w = w;
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Quaternion() {
        this->w = 1;
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }
    bool operator ==(Quaternion* a) {
        return fabs(w - a->w) <= QUATERNION_EPS &&
        fabs(x - a->x) <= QUATERNION_EPS &&
        fabs(y - a->y) <= QUATERNION_EPS &&
        fabs(z - a->z) <= QUATERNION_EPS;
    }
    bool operator ==(Quaternion a) {
        return *this == &a;
    }
    bool operator !=(Quaternion a) {
        return !(*this == &a);
    }
    bool operator !=(Quaternion* a) {
        return !(*this == a);
    }

    float lengthSquared();
    /**
    * Calculates the rotation vector and angle of a quaternion.
    * @param output
    *      A 3D vector of the quaternion rotation axis.
    * @return
    *      The rotation angle in radians.
    */
    float toAxisAngle(Vector3* output);
    float* toMat4();
    static void rotate(Quaternion* q, Vector3* v, Vector3* output);

    /**
    * Set the quaternion to the equivalent of axis-angle rotation.
    * @param axis
    *      The axis of the rotation (should be normalized).
    * @param angle
    *      Rotation angle in radians.
    */
    static void fromAxisAngle(Vector3* axis, float angle, Quaternion* output);

    /**
    * Interpolates between two quaternions.
    * @param t
    *      Interpolation between the two quaternions [0, 1].
    *      0 is equal with q1, 1 is equal with q2, 0.5 is the middle between q1 and q2.
    */
    static void slerpUnclamped(Quaternion* a, Quaternion* b, float t, Quaternion* output);
    static void slerp(Quaternion* q1, Quaternion* q2, float t, Quaternion* output); // delete original
    static void fromToRotation(Vector3* from, Vector3* to, Quaternion* output);
    static void lookRotation(Vector3* forward, Vector3* up, Quaternion* out);

    /**
    * Print the quaternion to a given file (e.g., stderr).
    */
    void fprint(FILE* file);

    /**
    * Set the quaternion to the equivalent a rotation around the X-axis.
    * @param angle
    *      Rotation angle in radians.
    */
    static void fromXRotation(float angle, Quaternion* output);

    /**
    * Set the quaternion to the equivalent a rotation around the Y-axis.
    * @param angle
    *      Rotation angle in radians.
    */
    static void fromYRotation(float angle, Quaternion* output);

    /**
    * Set the quaternion to the equivalent a rotation around the Z-axis.
    * @param angle
    *      Rotation angle in radians.
    */
    static void fromZRotation(float angle, Quaternion* output);

    /**
    * Calculates the norm of a given quaternion:
    * norm = sqrt(w*w + v1*v1 + v2*v2 + v3*v3)
    */
    float length();

    /**
    * Normalizes the quaternion.
    */
    void normalize(Quaternion* output);
    void normalize();

    /**
    * Calculates the conjugate of the quaternion: (w, -v)
    */
    static void conjugate(Quaternion* q, Quaternion* output);
    static void rotateInverse(Quaternion* q, Vector3* v, Vector3* output);

    void print();
} Quaternion;

static Quaternion IdentityQuaternion = {1, 0, 0, 0};
