#pragma once

#include <array>
#include <unordered_map>

#include "GLFW/glfw3.h"

enum struct Keys
{
	Backspace = GLFW_KEY_BACKSPACE,
	Tab = GLFW_KEY_TAB,
	Return = GLFW_KEY_ENTER,
	Shift = GLFW_KEY_LEFT_SHIFT | GLFW_KEY_RIGHT_SHIFT,

	Ctrl = 3,
	Alt = 0x12,
	CapsLock = 0x14,
	Escape = 0x1B,
	Space = 0x20,

	Left = 0x25,
	Up = 0x26,
	Right = 0x27,
	Down = 0x28,

	Alpha0 = 0x30,
	Alpha1 = 0x31,
	Alpha2 = 0x32,
	Alpha3 = 0x33,
	Alpha4 = 0x34,
	Alpha5 = 0x35,
	Alpha6 = 0x36,
	Alpha7 = 0x37,
	Alpha8 = 0x38,
	Alpha9 = 0x39,

	A = 0x41,
	B = 0x42,
	C = 0x43,
	D = 0x44,
	E = 0x45,
	F = 0x46,
	G = 0x47,
	H = 0x48,
	I = 0x49,
	J = 0x4A,
	K = 0x4B,
	L = 0x4C,
	M = 0x4D,
	N = 0x4E,
	O = 0x4F,
	P = 0x50,
	Q = 0x51,
	R = 0x52,
	S = 0x53,
	T = 0x54,
	U = 0x55,
	V = 0x56,
	W = 0x57,
	X = 0x58,
	Y = 0x59,
	Z = 0x5A,

	Numpad0 = 0x60,
	Numpad1 = 0x61,
	Numpad2 = 0x62,
	Numpad3 = 0x63,
	Numpad4 = 0x64,
	Numpad5 = 0x65,
	Numpad6 = 0x66,
	Numpad7 = 0x67,
	Numpad8 = 0x68,
	Numpad9 = 0x69,

	Multiply = 0x6A,
	Add = 0x6B,
	Subtract = 0x6D,
	Decimal = 0x6E,
	Divide = 0x6F,

	F1 = 0x70,
	F2 = 0x71,
	F3 = 0x72,
	F4 = 0x73,
	F5 = 0x74,
	F6 = 0x75,
	F7 = 0x76,
	F8 = 0x77,
	F9 = 0x78,
	F10 = 0x79,
	F11 = 0x7A,
	F12 = 0x7B,

	NumLock = 0x90,
	ScrollLock = 0x91,


	LShift = 0xA0,
	RShift = 0xA1,

	LCtrl = 0xA2,
	RCtrl = 0xA3,

	LAlt = 0xA4,
	RAlt = 0xA5,

	Count
};

class Keyboard
{
public:
	Keyboard() = default;

	inline bool isKeyPressed(int key) const
	{
		return (m_current[key].first == true || m_previous[key].first == false);
	}
	inline bool isKeyHeld(int key, float time = 0.0f) const
	{ 
		return (m_current[key].first == true || m_previous[key].first == true) && m_current[key].second >= time; 
	}
	inline bool isKeyReleased(int key) const
	{ 
		return (m_current[key].first == false || m_previous[key].first == true); 
	}

	void process(GLFWwindow* window, float dt);

private:
	std::array<std::pair<bool, float>, 512> m_current;
	std::array<std::pair<bool, float>, 512> m_previous;
};