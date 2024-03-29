#include "window.h"
#include "CSCIx229.h"
#include "vector2.h"

double Window::asp = 0;
Vector2 Window::windowSize = Vector2Zero;

void Window::reshape(int width, int height) {
    windowSize = new Vector2(width, height);
    asp = (height > 0) ? (float)width / height : 1;
    glViewport(0, 0, RES * width, RES * height);
    //glLoadIdentity();
}