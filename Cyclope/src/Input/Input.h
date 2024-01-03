#ifndef INPUT_H
#define INPUT_H

#include "Core.h"

#include "glad.h"
#include "glfw3.h"

#include "../Maths/Maths.h"

namespace Cyclope {

	enum class CYCLOPE_API Key
	{
		SPACE = 32,
		APOSTROPHE = 39,  /* ' */
		COMMA = 44,  /* , */
		MINUS = 45, /* - */
		PERIOD = 46,  /* . */
		SLASH = 47,  /* / */
		Key_0 = 48,
		Key_1 = 49,
		Key_2 = 50,
		Key_3 = 51,
		Key_4 = 52,
		Key_5 = 53,
		Key_6 = 54,
		Key_7 = 55,
		Key_8 = 56,
		Key_9 = 57,
		SEMICOLON = 59,  /* ; */
		EQUAL = 61,  /* = */
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79, //I gave up tabbing here
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,
		LEFT_BRACKET = 91, /* [ */
		BACKSLASH = 92,  /* \ */
		RIGHT_BRACKET = 93,  /* ] */
		GRAVE_ACCENT = 96, /* ` */
		WORLD_1 = 161, /* non-US #1 */
		WORLD_2 = 162, /* non-US #2 */
		ESCAPE = 256,
		ENTER = 257,
		TAB = 258,
		BACKSPACE = 259,
		INSERT = 260,
		DELETE_KEY = 261,// had to add _KEY because of a macro collision in a random file when adding Log.h
		RIGHT = 262,
		LEFT = 263,
		DOWN = 264,
		UP = 265,
		PAGE_UP = 266,
		PAGE_DOWN = 267,
		HOME = 268,
		END = 269,
		CAPS_LOCK = 280,
		SCROLL_LOCK = 281,
		NUM_LOCK = 282,
		PRINT_SCREEN = 283,
		PAUSE = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,
		KP_0 = 320,
		KP_1 = 321,
		KP_2 = 322,
		KP_3 = 323,
		KP_4 = 324,
		KP_5 = 325,
		KP_6 = 326,
		KP_7 = 327,
		KP_8 = 328,
		KP_9 = 329,
		KP_DECIMAL = 330,
		KP_DIVIDE = 331,
		KP_MULTIPLY = 332,
		KP_SUBTRACT = 333,
		KP_ADD = 334,
		KP_ENTER = 335,
		KP_EQUAL = 336,
		LEFT_SHIFT = 340,
		LEFT_CONTROL = 341,
		LEFT_ALT = 342,
		LEFT_SUPER = 343,
		RIGHT_SHIFT = 344,
		RIGHT_CONTROL = 345,
		RIGHT_ALT = 346,
		RIGHT_SUPER = 347,
		MENU = 348,
	};

	enum class CYCLOPE_API Button {
		Button_1 = 0,
		Button_2 = 1,
		Button_3 = 2,
		Button_4 = 3,
		Button_5 = 4,
		Button_6 = 5,
		Button_7 = 6,
		Button_8 = 7,
		Button_LAST = Button_8,
		Button_LEFT = Button_1,
		Button_RIGHT = Button_2,
		Button_MIDDLE = Button_3,
	};

	enum class CYCLOPE_API CursorMode {
		Hidden = GLFW_CURSOR_DISABLED,
		Shown = GLFW_CURSOR_NORMAL
	};

	class CYCLOPE_API Input {

	public:

		Input() = delete;

		static void SetCursorMode(CursorMode mode);

		//checks if the given key is pressed
		static bool KeyPressed(Key key);

		//checks if the given mouse _Button is pressed
		static bool ButtonPressed(Button button);

		static float MouseX();

		static float MouseY();

		static Vector2 GetDeltaScroll();

	private:
		//The window from which the Input is read from
		static GLFWwindow* s_window;

		//sets the window from which to read the input from
		static void SetWindow(GLFWwindow* window);

		static float s_mx;
		static float s_my;

		static Vector2 m_scroll;

		friend class Application;
		friend class Window;

	};

}

#endif