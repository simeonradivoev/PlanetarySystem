#include "FlyCamera.h"
#include "GLFW\glfw3.h">
#include <cstdio>
#include <list>
#include <glm\glm.hpp>
#include <glm\gtx\quaternion.hpp>
#include "Core/Input.h"
#include "Core/Time.h"

double warpSpeedEase = 0.5;
double maxWarpSpeed = 3;

FlyCamera::FlyCamera(double fov, double aspect, double zNear, double zFar) : Camera(fov, aspect, zNear, zFar)
{
	m_viewRotation.x = 180;
	m_viewRotation.y = 15;
}

void FlyCamera::OnRender(Display& display){
	//manage camera input
	ManageInput(display);
}

void FlyCamera::ManageInput(Display& display){
	//move the camera only if the mouse is visible
	if (!Input::MouseVisible || (Input::GetCurrentEvent().mouseAction == GLFW_DRAGG && Input::GetCurrentEvent().mouseButton == GLFW_MOUSE_BUTTON_2))
	{
		m_viewRotation += Input::GetMouseDelta();

		glm::dquat HRotation = glm::angleAxis(m_viewRotation.x * m_sensitivity, glm::dvec3(0, 1, 0));
		glm::dquat VRotation = glm::angleAxis(m_viewRotation.y * m_sensitivity, glm::dvec3(1, 0, 0) * HRotation);

		transform.rotation = m_initialRotation * HRotation * VRotation;
	}

	float speed = m_speed;
	m_velocity = glm::dvec3(0, 0, 0);

	//Shift Hyper speed
	if (glfwGetKey(display.GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
		if (m_warpSpeedMultiplayer < maxWarpSpeed)
			m_warpSpeedMultiplayer += Time::GetDeltaTime(TIME_DELTA_UNSCALSED) * warpSpeedEase;

		speed = m_warpSpeedMultiplayer;
	}
	else
	{
		m_warpSpeedMultiplayer = 1;
	}

	m_velocity.x = Input::GetHorizontalInput(INPUT_SMOOTH) * speed * Time::GetDeltaTime(TIME_DELTA_UNSCALSED);
	m_velocity.z = Input::GetVerticalInput(INPUT_SMOOTH) * speed  * Time::GetDeltaTime(TIME_DELTA_UNSCALSED);
	transform.position -= m_velocity * transform.rotation;
}


FlyCamera::~FlyCamera()
{
}
