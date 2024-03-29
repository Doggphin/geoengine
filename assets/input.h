#ifndef PROJECT_INPUT
#define PROJECT_INPUT

#include "vector2.h"

struct Input_InputMap {
	Input_InputMap(){}
	int esc; int a; int b; int c; int d; int e; int f; int g; int h; int i; int j; int k; int l; int m; int n; int o; int p; int q; int r; int s; int t; int u; int v; int w; int x; int y; int z; int space;
};

enum Input_Keycode {
	KEYCODE_UNUSED, KEYCODE_ESC, KEYCODE_A, KEYCODE_B, KEYCODE_C, KEYCODE_D, KEYCODE_E, KEYCODE_F, KEYCODE_G, KEYCODE_H, KEYCODE_I, KEYCODE_J, KEYCODE_K, KEYCODE_L, KEYCODE_M, KEYCODE_N, KEYCODE_O, KEYCODE_P, KEYCODE_Q, KEYCODE_R, KEYCODE_S, KEYCODE_T, KEYCODE_U, KEYCODE_V, KEYCODE_W, KEYCODE_X, KEYCODE_Y, KEYCODE_Z, KEYCODE_SPACE
};

class Input {
	public:
		static int allowInputs;
		static struct Vector2 mousePosition;
		static struct Vector2 mouseDeltaBuffer;
		Input(){}
		static void setKeyValue(Input_Keycode keycode, int value, int buffered);
		static int isKeyDownSwitch(Input_Keycode key, int buffered);
		static  Input_Keycode charToKeycode(unsigned char ch);

		static int getKey(Input_Keycode key);
		static int getKeyBuffered(Input_Keycode key);
		static void setBufferToCurrent();
		static void setKeyDown(unsigned char ch, int x, int y);
		static void setKeyUp(unsigned char ch, int x, int y);
		static void setMousePosition(Vector2* newPos);
		static void getMousePosition(Vector2* output);
		static void addMouseDelta(Vector2* delta);
		static void clearMouseDelta();
		static void clearMousePosition();
		static void getMouseDelta(Vector2* output);
		static void passive(int x, int y);
		static void setCursorLocked(int locked);
		static void getJoystick(Input_Keycode up, Input_Keycode right, Input_Keycode down, Input_Keycode left, int buffered, Vector2* &output);
		static void getJoystickNormalized(Input_Keycode up, Input_Keycode right, Input_Keycode down, Input_Keycode left, int buffered, Vector2* &output);
};
// include "key pressed" subscriber/listener thing here

#endif