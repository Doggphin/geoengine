// MOST OF THIS IS BORROWED FROM ONLINE

// Copyright (C) 2022 Martin Weigel <mail@MartinWeigel.com>
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
 * @file    Quaternion.c
 * @brief   A basic quaternion library written in C
 * @date    2022-05-16
 */
#include "Quaternion.h"
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#ifndef M_PI
    #define M_PI (3.14159265358979323846)
#endif

float Quaternion::lengthSquared() {
    return w * w + x * x + y * y + z * z;
}

void Quaternion::fprint(FILE* file)
{
    fprintf(file, "(%.3f, %.3f, %.3f, %.3f)",
        w, x, y, z);
}

void Quaternion::fromAxisAngle(Vector3* axis, float angle, Quaternion* output)
{
    assert(output != NULL);
    // Formula from http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/
    output->w = cos(angle / 2.0);
    float c = sin(angle / 2.0);
    output->x = c * axis->x;
    output->y = c * axis->y;
    output->z = c * axis->z;
}

float Quaternion::toAxisAngle(Vector3* output)
{
    assert(output != NULL);
    // Formula from http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToAngle/
    float angle = 2.0 * acos(w);
    float divider = sqrt(1.0 - w * w);

    if(divider != 0.0) {
        // Calculate the axis
        output->x = x / divider;
        output->y = y / divider;
        output->z = z / divider;
    } else {
        // Arbitrary normalized axis
        output->x = 1;
        output->y = 0;
        output->z = 0;
    }
    return angle;
}

void Quaternion::fromXRotation(float angle, Quaternion* output)
{
    assert(output != NULL);
    Vector3 axis(1, 0, 0);
    Quaternion::fromAxisAngle(&axis, angle, output);
}

void Quaternion::fromYRotation(float angle, Quaternion* output)
{
    assert(output != NULL);
    Vector3 axis(0, 1, 0);
    Quaternion::fromAxisAngle(&axis, angle, output);
}

void Quaternion::fromZRotation(float angle, Quaternion* output)
{
    assert(output != NULL);
    Vector3 axis(0, 0, 1);
    Quaternion::fromAxisAngle(&axis, angle, output);
}


void Quaternion::conjugate(Quaternion* q, Quaternion* output)
{
    assert(output != NULL);
    output->w = q->w;
    output->x = -q->x;
    output->y = -q->y;
    output->z = -q->z;
}

float Quaternion::length()
{
    return sqrt(lengthSquared());
}

void Quaternion::normalize(Quaternion* output)
{
    assert(output != NULL);
    float len = length();
    output->w = w/len;
    output->x = x/len;
    output->y = y/len;
    output->z = z/len;
}
void Quaternion::normalize() {
    this->normalize(this);
}

void Quaternion::rotate(Quaternion* q, Vector3* v, Vector3* output)
{
    assert(output != NULL);
    /*
    // https://stackoverflow.com/questions/22497093/faster-quaternion-vector-multiplication-doesnt-work

    float tx = 2 * (q->y * v->z - q->z * v->y);
    float ty = 2 * (q->z * v->x - q->x * v->z);
    float tz = 2 * (q->x * v->y - q->y * v->x);

    output->x = v->x + q->w * tx + (q->y * tz - q->z * ty);
    output->y = v->y + q->w * ty + (q->z * tx - q->x * tz);
    output->y = v->z + q->w * tz + (q->x * ty - q->y * tx);
    */
    
    float ww = q->w * q->w;
    float xx = q->x * q->x;
    float yy = q->y * q->y;
    float zz = q->z * q->z;
    float wx = q->w * q->x;
    float wy = q->w * q->y;
    float wz = q->w * q->z;
    float xy = q->x * q->y;
    float xz = q->x * q->z;
    float yz = q->y * q->z;

    // Formula from http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/transforms/index.htm
    // p2.x = w*w*p1.x + 2*y*w*p1.z - 2*z*w*p1.y + x*x*p1.x + 2*y*x*p1.y + 2*z*x*p1.z - z*z*p1.x - y*y*p1.x;
    // p2.y = 2*x*y*p1.x + y*y*p1.y + 2*z*y*p1.z + 2*w*z*p1.x - z*z*p1.y + w*w*p1.y - 2*x*w*p1.z - x*x*p1.y;
    // p2.z = 2*x*z*p1.x + 2*y*z*p1.y + z*z*p1.z - 2*w*y*p1.x - y*y*p1.z + 2*w*x*p1.y - x*x*p1.z + w*w*p1.z;

    /*output->x = ww*v->x + 2*wy*v->z - 2*wz*v->y +
                xx*v->x + 2*xy*v->y + 2*xz*v->z -
                zz*v->x - yy*v->x;
    output->y = 2*xy*v->x + yy*v->y + 2*yz*v->z +
                2*wz*v->x - zz*v->y + ww*v->y -
                2*wx*v->z - xx*v->y;
    output->z = 2*xz*v->x + 2*yz*v->y + zz*v->z -
                2*wy*v->x - yy*v->z + 2*wx*v->y -
                xx*v->z + ww*v->z;*/
                
    struct Vector3 temp(
                ww*v->x + 2*wy*v->z - 2*wz*v->y +
                xx*v->x + 2*xy*v->y + 2*xz*v->z -
                zz*v->x - yy*v->x,
                2*xy*v->x + yy*v->y + 2*yz*v->z +
                2*wz*v->x - zz*v->y + ww*v->y -
                2*wx*v->z - xx*v->y,
                2*xz*v->x + 2*yz*v->y + zz*v->z -
                2*wy*v->x - yy*v->z + 2*wx*v->y -
                xx*v->z + ww*v->z
    );
    *output = temp;
}

void Quaternion::rotateInverse(Quaternion* q, Vector3* v, Vector3* output) {
    struct Quaternion conjugate;
    Quaternion::conjugate(q, &conjugate);
    Quaternion::rotate(&conjugate, v, output);
}

void Quaternion_slerp(Quaternion* q1, Quaternion* q2, float t, Quaternion* output)
{
    Quaternion result = IdentityQuaternion;

    // Based on http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
    float cosHalfTheta = q1->w*q2->w + q1->x*q2->x + q1->y*q2->y + q1->z*q2->z;

    // if q1=q2 or qa=-q2 then theta = 0 and we can return qa
    if (fabs(cosHalfTheta) >= 1.0) {
        *output = q1;
        return;
    }

    float halfTheta = acos(cosHalfTheta);
    float sinHalfTheta = sqrt(1.0 - cosHalfTheta*cosHalfTheta);
    // If theta = 180 degrees then result is not fully defined
    // We could rotate around any axis normal to q1 or q2
    if (fabs(sinHalfTheta) < QUATERNION_EPS) {
        result.w = (q1->w * 0.5 + q2->w * 0.5);
        result.x = (q1->x * 0.5 + q2->x * 0.5);
        result.y = (q1->y * 0.5 + q2->y * 0.5);
        result.z = (q1->z * 0.5 + q2->z * 0.5);
    } else {
        // Default quaternion calculation
        float ratioA = sin((1 - t) * halfTheta) / sinHalfTheta;
        float ratioB = sin(t * halfTheta) / sinHalfTheta;
        result.w = (q1->w * ratioA + q2->w * ratioB);
        result.x = (q1->x * ratioA + q2->x * ratioB);
        result.y = (q1->y * ratioA + q2->y * ratioB);
        result.z = (q1->z * ratioA + q2->z * ratioB);
    }
    *output = result;
}


void Quaternion::fromToRotation(Vector3* from, Vector3* to, Quaternion* output) {
    /*
    // https://arrowinmyknee.com/2021/02/10/how-to-get-rotation-from-two-vectors/
    float norm_u_norm_v = sqrt(Vector3_sqrmagnitude(from) * Vector3_sqrmagnitude(to));
    float cos_theta = Vector3_dot(from, to) / norm_u_norm_v;
    float half_cos = sqrt(0.5f * (1 + cos_theta));
    struct Vector3 w;
    Vector3_cross(from, to, &w);
    Vector3_div(&w, (norm_u_norm_v * 2 * half_cos), &w);
    Quaternion_set(half_cos, w.x, w.y, w.z, output);
    // https://stackoverflow.com/a/11741520
    */
    
    float k_cos_theta = Vector3::dot(from, to);
    float k = sqrt(from->sqrmagnitude() * to->sqrmagnitude());
    struct Vector3 cache;
    if (k_cos_theta / k == -1) {
        from->orthogonal(&cache);
        cache.normalize();
        output->w = 0;
        output->x = cache.x;
        output->y = cache.y;
        output->z = cache.z;
    }
    else {
        Vector3::cross(from, to, &cache);
        output->w = k_cos_theta + k;
        output->x = cache.x;
        output->y = cache.y;
        output->z = cache.z;
        output->normalize();
    }
    
}

// https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
float* Quaternion::toMat4() {

    float* buf = (float*)malloc(sizeof(float) * 16);
    float x = -this->x;
    float y = -this->y;
    float z = -this->z;
    float w = this->w;
    buf[0] = 1 - 2*y*y - 2*z*z;
    buf[1] = 2*x*y - 2*z*w;
    buf[2] = 2*x*z + 2*y*w;
    buf[3] = 0;

    buf[4] = 2*x*y + 2*z*w;
    buf[5] = 1 - 2*x*x - 2*z*z;
    buf[6] = 2*y*z - 2*x*w;
    buf[7] = 0;

    buf[8] = 2*x*z - 2*y*w;
    buf[9] = 2*y*z + 2*x*w;
    buf[10] = 1 - 2*x*x - 2*y*y;
    buf[11] = 0;

    buf[12] = 0;
    buf[13] = 0;
    buf[14] = 0;
    buf[15] = 1;

    /*
    float ret[] = {
        1 - 2*(y*y - z*z),      2*(x*y - z*w),          2*(x*z + y*w),      0,
        2*(x*y) + 2*(z*w),      1 - 2*(x*x - z*z),      2*(y*z - x*w),      0,
        2*(x*z - y*w),          2*(y*z + x*w),          1 - 2*(x*x - y*y),  0,
        0,                      0,                      0,                  1
    };
    */
    //printf("The first row SHOULD be %.6f %.6f %.6f %.6f \n", ret[0], ret[1], ret[2], ret[3]);
    //buf = ret;
    return buf;
}

/*
void Quaternion_fromAxisAngle(Vector3* v, float angle, Quaternion* output) {
    float factor = sin(angle/2);
    Quaternion_set(cos(angle/2), v->x * factor, v->y * factor, v->z * factor, output);
    Quatenrion_normalize(output, output); 
}
*/

void Quaternion::print() {
    printf("(%.6f, %.6f, %.6f, %.6f)\n", w, x, y, z);
}

// https://gist.github.com/HelloKitty/91b7af87aac6796c3da9
// untested
void Quaternion::slerpUnclamped(Quaternion* a, Quaternion* b, float t, Quaternion* output) {
    Quaternion newB = Quaternion();
    newB = b;
    if(a->lengthSquared() == 0) {
        if(b->lengthSquared() == 0) {
            *output = IdentityQuaternion;
        }
        *b = output;
    }
    else if(newB.lengthSquared() == 0) {
        *output = newB;
    }

    Vector3 c = {a->x, a->y, a->z};
    Vector3 d = {b->x, b->y, b->z};

    float cosHalfAngle = a->w * b->w + Vector3::dot(&c, &d);
;
    if(cosHalfAngle >= 1 || cosHalfAngle <= -1) {
        *output = a;
    }
    else if(cosHalfAngle < 0) {
        newB.x = -newB.x;
        newB.y = -newB.y;
        newB.z = -newB.z;
        newB.w = -newB.w;
        cosHalfAngle = -cosHalfAngle;
    }

    float blendA;
    float blendB;
    if(cosHalfAngle < 0.99f) {
        float halfAngle = acos(cosHalfAngle);
        float sinHalfAngle = sin(halfAngle);
        float oneOverSinHalfAngle = 1/sinHalfAngle;
        blendA = sin(halfAngle * (1-t)) * oneOverSinHalfAngle;
        blendB = sin(halfAngle * t) * oneOverSinHalfAngle;
    }
    else {
        blendA = 1-t;
        blendB = t;
    }

    *output = Quaternion(blendA * a->w + blendB * b->w, blendA * a->x + blendB * b->x, blendA * a->y + blendB * b->y, blendA * a->z + blendB * b->z);
    if(output->lengthSquared() > 0) {
        output->normalize();
    } else {
        *output = IdentityQuaternion;
    }
}

/*
void Quaternion_slerpP(Quaternion* a, Quaternion* b, float t, Quaternion* output) {
    // Clamp t between 0 and 1
    Quaternion_slerpUnclamped(a, b, t > 1 ? 1 : t < 0 ? 0 : t, output);
}
*/

// https://gist.github.com/HelloKitty/91b7af87aac6796c3da9
void Quaternion::lookRotation(Vector3* forward, Vector3* up, Quaternion* out) {
    Vector3 forwardN(forward);
    forwardN.normalize();
    Vector3 right = new Vector3();
    Vector3::cross(up, &forwardN, &right);
    Vector3 upN = new Vector3();
    Vector3::cross(&forwardN, &right, &upN);
    float m00 = right.x;
    float m01 = right.y;
    float m02 = right.z;
    float m10 = upN.x;
    float m11 = upN.y;
    float m12 = upN.z;
    float m20 = forwardN.x;
    float m21 = forwardN.y;
    float m22 = forwardN.z;

    float num8 = (m00 + m11) + m22;
    Quaternion quat = IdentityQuaternion;
    if(num8 > 0) {
        float num = sqrt(num8 + 1);
        quat.w = num * .5;
        num = .5/num;
        quat.x = (m12-m21) * num;
        quat.y = (m20-m02) * num;
        quat.z = (m01-m10) * num;
        *out = quat;
        return;
    }
    if(m00 >= m11 && m00 >= m22) {
        float num7 = sqrt(((1 + m00) - m11) - m22);
        float num4 = .5 / num7;
        quat.x = .5 * num7;
        quat.y = (m01 + m10) * num4;
        quat.z = (m02 + m20) * num4;
        quat.w = (m12 - m21) * num4;
        *out = quat;
        return;
    }
    if(m11 > m22) {
        float num6 = sqrt(((1 + m11) - m00) - m22);
        float num3 = .5 / num6;
        quat.x = (m10+m01) * num3;
        quat.y = .5 * num6;
        quat.z = (m21 + m12) * num3;
        quat.w = (m20 - m02) * num3;
        *out = quat;
        return;
    }
    float num5 = sqrt(((1 + m22) - m00) - m11);
    float num2 = 0.5/num5;
    quat.x = (m20+m02) * num2;
    quat.y = (m21+m12) * num2;
    quat.z = 0.5 * num5;
    quat.w = (m01 - m10) * num2;
    *out = quat;
    return;

}