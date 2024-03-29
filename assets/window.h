#ifndef WINDOW_H
#define WINDOW_H

#include "vector2.h"

class Window {
    public:
        static struct Vector2 windowSize;
        static double asp;
        static void reshape(int width, int height);
};

#endif