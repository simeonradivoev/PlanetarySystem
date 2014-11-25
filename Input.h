#pragma once
#include "display.h"
#include "glm\glm.hpp"
#include "glm\gtx\compatibility.hpp"
#include "GLFW\glfw3.h"


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
private:
	static glm::vec2 HVInputRaw;
	static glm::vec2 HVInputSmooth;
};

