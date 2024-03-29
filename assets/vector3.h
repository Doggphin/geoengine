#ifndef VECTOR3_H
#define VECTOR3_H

#include <stdint.h>
#include <cmath>

/**
 * Maximum floating point difference that is considered as equal.
 */
#define VECTOR3_EPS (1e-4)


typedef struct Vector3 {
	private:
		float rad2deg(float rad);
	public:
		float x;
		float y;
		float z;
		Vector3() {
			this->x = 0;
			this->y = 0;
			this->z = 0;
		}
		Vector3(float x, float y, float z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}
		Vector3(Vector3* v) {
			this->x = v->x;
			this->y = v->y;
			this->z = v->z;
		}
		void operator =(Vector3* a) { x = a->x; y = a->y; z = a->z; }
		void operator =(Vector3 a) { *this = &a; }
		Vector3 operator *(float a) { return new Vector3(x * a, y * a, z * a); }
		Vector3 operator /(float a) { return new Vector3(x / a, y / a, z / a); }
		Vector3 operator +(Vector3* a) { return new Vector3(x + a->x, y + a->y, z + a->z); }
		Vector3 operator +(Vector3 a) { return new Vector3(x + a.x, y + a.y, z + a.z); }
		Vector3 operator +(float a) { return new Vector3(x + a, y + a, z + a); }
		Vector3 operator *(Vector3* a) { return new Vector3(x * a->x, y * a->y, z * a->z); }
		Vector3 operator *(Vector3 a) { return new Vector3(x * a.x, y * a.y, z * a.z); }
		/*Vector3 operator *(Quaternion* a) {
			 Vector3 ret = new Vector3();
			 Quaternion::rotate(a, this, &ret);
			 return ret;
			 }
		Vector3 operator *(Quaternion a) {
			Vector3 ret = new Vector3();
			 Quaternion::rotate(&a, this, &ret);
			 return ret;
		}*/
		Vector3 operator -(Vector3* a) { return new Vector3(x - a->x, y - a->y, z - a->z); }
		Vector3 operator -(Vector3 a) { return new Vector3(x - a.x, y - a.y, z - a.z); }
		Vector3 operator -(float a) { return new Vector3(x - a, y - a, z - a); }
		Vector3 operator /(Vector3* a) { return new Vector3(x / a->x, y / a->y, z / a->z); }
		Vector3 operator /(Vector3 a) { return new Vector3(x / a.x, y / a.y, z / a.z); }
		void operator *=(float a)  {x*=a; y*=a; z*=a; }
		void operator /=(float a) {x/=a; y/=a; z/=a; }
		void operator *=(Vector3* a) { x = x * a->x; y = y * a->y; z = z * a->z; }
		void operator *=(Vector3 a) { *this *= &a; }
		/*void operator *=(Quaternion* a) {
			struct Vector3 temp(x, y, z);
			struct Vector3 buffer;
			Quaternion::rotate(a, &temp, &buffer);
			x = buffer.x;
			y = buffer.y;
			z = buffer.z;
		}
		void operator *=(Quaternion a) {
			*this *= &a;
		}*/
		void operator /=(Vector3* a) { x = x / a->x; y = y / a->y; z = z / a->z; }
		void operator /=(Vector3 a) { *this /= &a; }
		void operator +=(Vector3* a) {x = x + a->x; y = y + a->y; z = z + a->z; }
		void operator +=(Vector3 a) { *this += &a;}
		void operator +=(float a) {x = x + a; y = y + a; z = z + a; }
		void operator -=(Vector3* a) {x = x - a->x; y = y - a->y; z = z - a->z; }
		void operator -=(float a) {x = x - a; y = y - a; z = z - a; }
		void operator -=(Vector3 a) { *this -= &a;}
		bool operator ==(Vector3* a) { return fabs(x - a->x) <= VECTOR3_EPS && fabs(y - a->y) <= VECTOR3_EPS && fabs(z - a->z) <= VECTOR3_EPS; }
		bool operator ==(Vector3 a) { return *this == &a; }
		bool operator !=(Vector3 a) { return !(*this == &a); }
		bool operator !=(Vector3* a) { return !(*this == *a); }


		float sqrmagnitude();
		float magnitude();
		float max();
		float min();
		void normalize();
		void normalize(Vector3* output);
		Vector3 abs();
		void abs(Vector3* output);
		float dot(Vector3* v2);
		static float dot(Vector3* v1, Vector3* v2);
		float anglerad(Vector3* v2);
		float angledeg(Vector3* v2);
		float sqrdistance(Vector3* point2);
		float distance(Vector3* point2);
		void print();

		static void project(Vector3* from, Vector3* onto, Vector3* output);
		//static Vector3* project(Vector3* from, Vector3* onto);
		void project(Vector3* onto);
		static void lerp(Vector3* from, Vector3* to, float t, Vector3* output);
		//static Vector3* lerp(Vector3* from, Vector3* to, float t);
		void lerp(Vector3* to, float t);
		static void lerpclamped(Vector3* from, Vector3* to, float t, Vector3* output);
		//static Vector3* lerpclamped(Vector3* from, Vector3* to, float t);
		void lerpclamped(Vector3* to, float t);
		static void cross(Vector3* a, Vector3* b, Vector3* output);
		//static Vector3* cross(Vector3* a, Vector3* b);
		void cross(Vector3* b);
		void orthogonal(Vector3* b, Vector3* output);
		//static Vector3* orthogonal(Vector3* a, Vector3* b);
		void orthogonal(Vector3* b);
		static void cumMin(Vector3* a, Vector3* b, Vector3* output);
		static void cumMax(Vector3* a, Vector3* b, Vector3* output);
} Vector3;


static const Vector3 Vector3Up = {0, 1, 0};
static const Vector3 Vector3Down = {0, -1, 0};
static const Vector3 Vector3Forward = {0, 0, -1};
static const Vector3 Vector3Backward = {0, 0, 1};
static const Vector3 Vector3Right = {1, 0, 0};
static const Vector3 Vector3Left = {1, 0, 0};
static const Vector3 Vector3Zero = {0, 0, 0};
static const Vector3 Vector3One = {1, 1, 1};

#endif