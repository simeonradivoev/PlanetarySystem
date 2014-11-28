#pragma once
#ifndef INPUT_H
#define INPUT_H

#include "display.h"
#include "glm\glm.hpp"
#include "glm\gtx\compatibility.hpp"
#include "GLFW\glfw3.h"
using namespace glm;

#define INPUT_SMOOTH 0
#define INPUT_RAW 1

class Input
{
public:
	Input(Display& display);
	~Input();
	static bool MouseVisible;
	void ManageInput(Display& display);
	static float GetHorizontalInput(int type);
	static float GetVerticalInput(int type);
	static vec2 GetMouseDelta(){ return m_mouseDelta; }
private:
	static vec2 HVInputRaw;
	static vec2 HVInputSmooth;
	static vec2 m_mouseDelta;
	static vec2 m_lastMousePosition;
};
#endif //INPUT_H