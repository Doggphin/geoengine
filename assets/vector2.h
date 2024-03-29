#ifndef VECTOR2_H
#define VECTOR2_H
#include <math.h>

#define VECTOR2_EPS (1e-4)

class Vector2 {
    public:
        float x;
        float y;
        Vector2() {
            this->x = 0;
            this->y = 0;
        }
		Vector2(float x, float y) {
			this->x = x;
			this->y = y;
		}
		Vector2(Vector2* v) {
			this->x = v->x;
			this->y = v->y;
		}
		void operator =(Vector2* a) { x = a->x; y = a->y; }
		void operator =(Vector2 a) { *this = &a; }
		Vector2 operator *(float a) { return new Vector2(x * a, y * a); }
		Vector2 operator /(float a) { return new Vector2(x / a, y / a); }
		Vector2 operator +(Vector2* a) { return new Vector2(x + a->x, y + a->y); }
		Vector2 operator +(Vector2 a) { return new Vector2(x + a.x, y + a.y); }
		Vector2 operator *(Vector2* a) { return new Vector2(x * a->x, y * a->y); }
		Vector2 operator *(Vector2 a) { return new Vector2(x * a.x, y * a.y); }
		Vector2 operator -(Vector2* a) { return new Vector2(x - a->x, y - a->y); }
		Vector2 operator -(Vector2 a) { return new Vector2(x - a.x, y - a.y); }
		Vector2 operator /(Vector2* a) { return new Vector2(x / a->x, y / a->y); }
		Vector2 operator /(Vector2 a) { return new Vector2(x / a.x, y / a.y); }
		void operator *=(float a)  {x*=a; y*=a; }
		void operator /=(float a) {x/=a; y/=a; }
		void operator *=(Vector2* a) { x = x * a->x; y = y * a->y; }
		void operator *=(Vector2 a) { *this *= &a; }
		void operator /=(Vector2* a) { x = x / a->x; y = y / a->y; }
		void operator /=(Vector2 a) { *this /= &a; }
		void operator +=(Vector2* a) {x = x + a->x; y = y + a->y; }
		void operator +=(Vector2 a) { *this += &a;}
		void operator -=(Vector2* a) {x = x - a->x; y = y - a->y; }
		void operator -=(Vector2 a) { *this -= &a;}
		bool operator ==(Vector2* a) { return fabs(x - a->x) <= VECTOR2_EPS && fabs(y - a->y) <= VECTOR2_EPS;}
		bool operator ==(Vector2 a) { return *this == &a; }
		bool operator !=(Vector2 a) { return !(*this == &a); }
		bool operator !=(Vector2* a) { return !(*this == *a); }

        void print();
        float sqrmagnitude();
        float magnitude();
};

static const Vector2 Vector2Up = {0, 1};
static const Vector2 Vector2Right = {1, 0};
static const Vector2 Vector2Down = {0, -1};
static const Vector2 Vector2Left = {-1, 0};
static const Vector2 Vector2Zero = {0, 0};
static const Vector2 Vector2One = {1, 1};

#endif