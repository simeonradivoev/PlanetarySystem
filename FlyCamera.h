#pragma once
#ifndef FLY_CAMERA_H
#define FLY_CAMERA_H

#include "camera.h"
#include "display.h"
#include <glm\glm.hpp>

class FlyCamera : public Camera
{
public:
	FlyCamera(double fov, double aspect, double zNear, double zFar);
	void RotateCamera(double angle, glm::dvec3 axis){ m_initialRotation *= glm::angleAxis(angle, axis); }
	void OnRender(Display& display) override;
	void ManageInput(Display& display);
	~FlyCamera();
private:
	glm::dvec3 m_velocity;
	glm::dquat m_initialRotation;
	glm::vec2 m_viewRotation;
	double m_warpSpeedMultiplayer = 1;
	double m_speed = 0.1f;
	double m_sensitivity = 3;
};
#endif //FLY_CAMERA_H