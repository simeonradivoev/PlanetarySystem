#include "Input.h"
#include "Time.h"
#include "../physics.h"

vec2 Input::HVInputRaw;
vec2 Input::HVInputSmooth;
vec2 Input::m_mouseDelta;
vec2 Input::m_lastMousePosition;
vec2 Input::m_mousePos;
Input::InputEvent Input::m_currentEvent;
bool Input::m_mouseIsPressed = false;
dvec3 Input::m_mouseWorldPlanePoition;

bool Input::MouseVisible;

Input::Input(Display& display)
{
	glfwSetKeyCallback(display.GetWindow(), Input::key_callback);
	glfwSetCursorPosCallback(display.GetWindow(), Input::mouse_pos_callback);
	glfwSetMouseButtonCallback(display.GetWindow(),Input::mouseButtons_callback);
}

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
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

	m_currentEvent.key = key;
	m_currentEvent.keyAction = action;
}

void Input::mouse_pos_callback(GLFWwindow* window, double x, double y)
{
	m_mousePos = vec2(x, y);
	m_currentEvent.mousePos = vec2(x, y);
}

void Input::mouseButtons_callback(GLFWwindow* window, int button, int action, int mod)
{
	if (action == GLFW_PRESS)
		m_mouseIsPressed = true;
	if (action == GLFW_RELEASE)
		m_mouseIsPressed = false;

	m_currentEvent.mouseAction = action;
	m_currentEvent.mouseButton = button;
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

bool Input::GetMouseButton(int button)
{
	return glfwGetMouseButton(Display::GetCurrentDisplay()->GetWindow(),button);
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

	m_mouseDelta = (m_mousePos - m_lastMousePosition) * (float)Time::GetDeltaTime(TIME_DELTA_RAW);
	Physics::IntersectsPlane(Camera::GetCurrentCamera()->ScreenToRay(m_mousePos), dvec3(), dvec3(0, 1, 0), &m_mouseWorldPlanePoition);

	m_currentEvent.mouseDelta = (m_mousePos - m_lastMousePosition);
	m_lastMousePosition = m_mousePos;
}

void Input::PollEvents()
{
	m_currentEvent.keyAction = GLFW_KEY_UNKNOWN;
	m_currentEvent.mouseAction = GLFW_KEY_UNKNOWN;
	if (Input::m_mouseIsPressed)
	m_currentEvent.mouseAction = GLFW_DRAGG;
	glfwPollEvents();
}

Input::~Input()
{
}
