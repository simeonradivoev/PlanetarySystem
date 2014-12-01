#pragma once
#ifndef INPUT_H
#define INPUT_H

#include "../display.h"
#include "glm\glm.hpp"
#include "glm\gtx\compatibility.hpp"
#include "GLFW\glfw3.h"
using namespace glm;

#define INPUT_SMOOTH 0
#define INPUT_RAW 1

class Input
{
public:
	struct InputEvent
	{
		int key = GLFW_KEY_UNKNOWN;
		glm::vec2 mousePos;
		int mouseButton = GLFW_KEY_UNKNOWN;
		int keyAction = GLFW_KEY_UNKNOWN;
		int mouseAction = GLFW_KEY_UNKNOWN;
	};

	Input(Display& display);
	~Input();
	static bool MouseVisible;
	void ManageInput(Display& display);
	void PollEvents();
	static float GetHorizontalInput(int type);
	static float GetVerticalInput(int type);
	static vec2 GetMouseDelta(){ return m_mouseDelta; }
	static vec2 GetMousePosition(){ return m_mousePos; }
	static bool GetMouseButton(int button);
	static const InputEvent GetCurrentEvent(){ return m_currentEvent; }
private:
	static InputEvent m_currentEvent;
	static vec2 HVInputRaw;
	static vec2 HVInputSmooth;
	static vec2 m_mousePos;
	static vec2 m_mouseDelta;
	static vec2 m_lastMousePosition;
	static void mouse_pos_callback(GLFWwindow* window, double x, double y);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtons_callback(GLFWwindow* window, int button, int action, int mod);
};
#endif //INPUT_H