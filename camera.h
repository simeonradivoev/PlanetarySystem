#pragma once
#include "transform.h"
#include "display.h"
#include "glm\glm.hpp"
#include "GLFW\glfw3.h"


class Camera
{
public:
	Camera(double fov, double aspect, double zNear, double zFar)
	{
		m_fov = fov;
		m_zNear = zNear;
		m_ZFar = zFar;
		m_aspectRatio = aspect;
	}

	inline glm::dmat4 GetViewProjection(Display& display) const
	{
		int width, height;
		glfwGetFramebufferSize(display.GetWindow(), &width, &height);
		return glm::perspective(m_fov, (double)width / (double)height, m_zNear, m_ZFar) * m_transform.GetModelRotationFirst();
	}

	_declspec(property(get = GetTransform, put = SetTransform)) Transform& transform;

	Transform& GetTransform(){ return m_transform; }
	void SetTransform(Transform& transform){ Camera::m_transform = transform; }
	virtual ~Camera();
private:
	Transform m_transform;
	double m_fov;
	double m_zNear;
	double m_ZFar;
	double m_aspectRatio;
};

