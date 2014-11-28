#include "Input.h"
#include "Time.h"

vec2 Input::HVInputRaw;
vec2 Input::HVInputSmooth;
vec2 Input::m_mouseDelta;
vec2 Input::m_lastMousePosition;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//enable and disable the mouse
	if (action == GLFW_PRESS){
		if (key == GLFW_KEY_ESCAPE){
			if (Input::MouseVisible){
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				Input::MouseVisible = false;
			}
			else
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				Input::MouseVisible = true;
			}
		}
	}
}

bool Input::MouseVisible;

Input::Input(Display& display)
{
	glfwSetKeyCallback(display.GetWindow(), key_callback);
}

float Input::GetHorizontalInput(int type){
	if (type == INPUT_SMOOTH){
		return HVInputSmooth.x;
	}
	else
	{
		return HVInputRaw.x;
	}

}

float Input::GetVerticalInput(int type){
	if (type == INPUT_SMOOTH){
		return HVInputSmooth.y;
	}
	else
	{
		return HVInputRaw.y;
	}
}

void Input::ManageInput(Display& display){

	HVInputRaw.x = 0;
	HVInputRaw.y = 0;

	if (glfwGetKey(display.GetWindow(), GLFW_KEY_W))
		HVInputRaw.y++;
	if (glfwGetKey(display.GetWindow(), GLFW_KEY_S))
		HVInputRaw.y--;
	if (glfwGetKey(display.GetWindow(), GLFW_KEY_A))
		HVInputRaw.x++;
	if (glfwGetKey(display.GetWindow(), GLFW_KEY_D))
		HVInputRaw.x--;


	HVInputSmooth = glm::lerp(HVInputSmooth, HVInputRaw, 0.1f);

	double x, y;
	glfwGetCursorPos(display.GetWindow(),&x,&y);

	m_mouseDelta = (vec2(x, y) - m_lastMousePosition) * (float)Time::GetDeltaTime(TIME_DELTA_RAW);
	m_lastMousePosition = vec2(x, y);
}

Input::~Input()
{
}
