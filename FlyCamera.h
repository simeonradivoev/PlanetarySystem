#pragma once
#include "camera.h"
#include "display.h"

class FlyCamera : public Camera
{
public:
	FlyCamera(double fov, double aspect, double zNear, double zFar);
	void ManageInput(Display& display);
	~FlyCamera();
private:
	glm::dvec3 m_velocity;
	double m_speed = 0.1f;
	double m_sensitivity = 3;
};

