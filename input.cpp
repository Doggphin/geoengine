#include "input.h"
#include "CSCIx229.h"
#include "window.h"
#include <assert.h>

static struct Input_InputMap current_inputs;
static struct Input_InputMap buffered_inputs;
static const struct Input_InputMap empty_inputs;

Vector2 Input::mousePosition = Vector2Zero;
Vector2 Input::mouseDeltaBuffer = Vector2Zero;
int Input::allowInputs = 0;

int lockCursor = 1;

#define MACRO_KEYCASE(A, B) {\
	case A:\
		B = value;\
}
void Input::setKeyValue(Input_Keycode keycode, int value, int buffered) {
	Input_InputMap* selected = buffered ? &buffered_inputs : &current_inputs;

	if (keycode == KEYCODE_UNUSED) {
		return;
	}
	switch (keycode) {
		case KEYCODE_ESC:
			if (value) { exit(0); }		// temp
			selected->esc = value;
			break;
		MACRO_KEYCASE(KEYCODE_A, selected->a)
		case KEYCODE_B:
			selected->b = value;
			break;
		case KEYCODE_C:
			selected->c = value;
			break;
		case KEYCODE_D:
			selected->d = value;
			break;
		case KEYCODE_E:
			selected->e = value;
			break;
		case KEYCODE_F:
			selected->f = value;
			break;
		case KEYCODE_G:
			selected->g = value;
			break;
		case KEYCODE_H:
			selected->h = value;
			break;
		case KEYCODE_I:
			selected->i = value;
			break;
		case KEYCODE_J:
			selected->j = value;
			break;
		case KEYCODE_K:
			selected->k = value;
			break;
		case KEYCODE_L:
			selected->l = value;
			break;
		case KEYCODE_M:
			selected->m = value;
			break;
		case KEYCODE_N:
			selected->n = value;
			break;
		case KEYCODE_O:
			selected->o = value;
			break;
		case KEYCODE_P:
			selected->p = value;
			break;
		case KEYCODE_Q:
			selected->q = value;
			break;
		case KEYCODE_R:
			selected->r = value;
			break;
		case KEYCODE_S:
			selected->s = value;
			break;
		case KEYCODE_T:
			selected->t = value;
			break;
		case KEYCODE_U:
			selected->u = value;
			break;
		case KEYCODE_V:
			selected->v = value;
			break;
		case KEYCODE_W:
			selected->w = value;
			break;
		case KEYCODE_X:
			selected->x = value;
			break;
		case KEYCODE_Y:
			selected->y = value;
			break;
		case KEYCODE_Z:
			selected->z = value;
			break;
		MACRO_KEYCASE(KEYCODE_SPACE, selected->space)
		default:
			printf("Unimplemented setInput return for given keycode\n");
			break;
	}
}

int Input::isKeyDownSwitch(Input_Keycode key, int buffered) {
	Input_InputMap* selected = buffered ? &buffered_inputs : &current_inputs;

	switch (key) {
		case KEYCODE_ESC:
			return selected->esc;
		case KEYCODE_A:
			return selected->a;
		case KEYCODE_B:
			return selected->b;
		case KEYCODE_C:
			return selected->c;
		case KEYCODE_D:
			return selected->d;
		case KEYCODE_E:
			return selected->e;
		case KEYCODE_F:
			return selected->f;
		case KEYCODE_G:
			return selected->g;
		case KEYCODE_H:
			return selected->h;
		case KEYCODE_I:
			return selected->i;
		case KEYCODE_J:
			return selected->j;
		case KEYCODE_K:
			return selected->k;
		case KEYCODE_L:
			return selected->l;
		case KEYCODE_M:
			return selected->m;
		case KEYCODE_N:
			return selected->n;
		case KEYCODE_O:
			return selected->o;
		case KEYCODE_P:
			return selected->p;
		case KEYCODE_Q:
			return selected->q;
		case KEYCODE_R:
			return selected->r;
		case KEYCODE_S:
			return selected->s;
		case KEYCODE_T:
			return selected->t;
		case KEYCODE_U:
			return selected->u;
		case KEYCODE_V:
			return selected->v;
		case KEYCODE_W:
			return selected->w;
		case KEYCODE_X:
			return selected->x;
		case KEYCODE_Y:
			return selected->y;
		case KEYCODE_Z:
			return selected->z;
		case KEYCODE_SPACE:
			return selected->space;
		default:
			printf("Unimplemented getInput return for given keycode\n");
			return 0;
	}
}


int Input::getKeyBuffered(Input_Keycode key) {
	return Input::isKeyDownSwitch(key, 1);
}
int Input::getKey(Input_Keycode key) {
	return Input::isKeyDownSwitch(key, 0);
}

#define CHARTOKEYCODECASE(UPPER, LOWER, KEYCODE) {\
	case UPPER:\
	case LOWER:\
		return KEYCODE;\
}
#define CHARTOKEYCODECASESIMP(KEY, KEYCODE) {\
	case KEY:\
		return KEYCODE;\
}

Input_Keycode Input::charToKeycode(unsigned char ch) {
	switch (ch) {
		CHARTOKEYCODECASE('A', 'a', KEYCODE_A);
		CHARTOKEYCODECASE('B', 'b', KEYCODE_B);
		CHARTOKEYCODECASE('C', 'c', KEYCODE_C);
		CHARTOKEYCODECASE('D', 'd', KEYCODE_D);
		CHARTOKEYCODECASE('E', 'e', KEYCODE_E);
		CHARTOKEYCODECASE('F', 'f', KEYCODE_F);
		CHARTOKEYCODECASE('G', 'g', KEYCODE_G);
		CHARTOKEYCODECASE('H', 'h', KEYCODE_H);
		CHARTOKEYCODECASE('I', 'i', KEYCODE_I);
		CHARTOKEYCODECASE('J', 'j', KEYCODE_J);
		CHARTOKEYCODECASE('K', 'k', KEYCODE_K);
		CHARTOKEYCODECASE('L', 'l', KEYCODE_L);
		CHARTOKEYCODECASE('M', 'm', KEYCODE_M);
		CHARTOKEYCODECASE('N', 'n', KEYCODE_N);
		CHARTOKEYCODECASE('O', 'o', KEYCODE_O);
		CHARTOKEYCODECASE('P', 'p', KEYCODE_P);
		CHARTOKEYCODECASE('Q', 'q', KEYCODE_Q);
		CHARTOKEYCODECASE('R', 'r', KEYCODE_R);
		CHARTOKEYCODECASE('S', 's', KEYCODE_S);
		CHARTOKEYCODECASE('T', 't', KEYCODE_T);
		CHARTOKEYCODECASE('U', 'u', KEYCODE_U);
		CHARTOKEYCODECASE('V', 'v', KEYCODE_V);
		CHARTOKEYCODECASE('W', 'w', KEYCODE_W);
		CHARTOKEYCODECASE('X', 'x', KEYCODE_X);
		CHARTOKEYCODECASE('Y', 'y', KEYCODE_Y);
		CHARTOKEYCODECASE('Z', 'z', KEYCODE_Z);
		CHARTOKEYCODECASESIMP(39, KEYCODE_SPACE)
		case 27:
			return KEYCODE_ESC;
		default:
			return KEYCODE_UNUSED;
	}
}

void Input::setBufferToCurrent() {
	buffered_inputs = current_inputs;
}

void Input::setKeyDown(unsigned char ch, int x, int y) {
	Input_Keycode keycode = Input::charToKeycode(ch);
	setKeyValue(keycode, 1, 0);
	setKeyValue(keycode, 1, 1);
}

void Input::setKeyUp(unsigned char ch, int x, int y) {
	Input::setKeyValue(Input::charToKeycode(ch), 0, 0);
}


void Input::setMousePosition(Vector2* newPos) {
	mousePosition = newPos;
}

void Input::clearMousePosition() {
	mousePosition = Vector2Zero;
}

void Input::getMousePosition(Vector2* output) {
	*output = mousePosition;
}

void Input::getMouseDelta(Vector2* output) {
	*output = mouseDeltaBuffer;
}

void Input::addMouseDelta(Vector2* delta) {
	if(allowInputs) {
		mouseDeltaBuffer += delta;
	}
}

void Input::clearMouseDelta() {
	mouseDeltaBuffer = Vector2Zero;
}

void Input::passive(int x, int y) {
	struct Vector2 windowSize = Window::windowSize;
	int relativeMouseX = x - windowSize.x / 2;
	int relativeMouseY = y - windowSize.y / 2;
	if(allowInputs) {		
		if (lockCursor) {
			// If clicking onto screen from outside, don't make mouse go crazy
			if(!(fabs(relativeMouseX) > 100 || fabs(relativeMouseY) > 100)) {
				mouseDeltaBuffer += new Vector2(relativeMouseX, relativeMouseY);
			}
			mousePosition = new Vector2(0,0);
			glutWarpPointer(windowSize.x / 2, windowSize.y / 2);
			Input::clearMousePosition();
		}
		else {
			mousePosition = new Vector2(relativeMouseX, relativeMouseY);
		}
	}
	else if(lockCursor) {
		glutWarpPointer(windowSize.x / 2, windowSize.y / 2);
		Input::clearMousePosition();
	}
}

void Input::setCursorLocked(int locked) {
	lockCursor = locked;
	glutSetCursor(locked ? GLUT_CURSOR_NONE : GLUT_CURSOR_LEFT_ARROW);
}

void Input::getJoystick(Input_Keycode up, Input_Keycode right, Input_Keycode down, Input_Keycode left, int buffered, Vector2* &output) {
	assert(output != NULL);
	output = new Vector2(Input::isKeyDownSwitch(right, buffered) - Input::isKeyDownSwitch(left, buffered), Input::isKeyDownSwitch(up, buffered) - Input::isKeyDownSwitch(down, buffered));
}

void Input::getJoystickNormalized(Input_Keycode up, Input_Keycode right, Input_Keycode down, Input_Keycode left, int buffered, Vector2* &output) {
	assert(output != NULL);
	Input::getJoystick(up, right, down, left, buffered, output);
	if(fabsf(output->x) == 1 && fabsf(output->y) == 1) {
		*output *= .7071;
	}
}