#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "transform.h"
#include "display.h"
#include "glm\glm.hpp"
#include "GLFW\glfw3.h"
using namespace glm;

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

	inline dmat4 GetViewProjection()
	{
		int width, height;
		glfwGetFramebufferSize(Display::GetCurrentDisplay()->GetWindow(), &width, &height);
		return glm::perspective(m_fov, (double)width / (double)height, m_zNear, m_ZFar) * m_transform.GetRotationMatrix() * m_transform.GetTranslationMatrix();
	}

	virtual void OnRender(Display& display){}

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
#endif	//CAMERA_H
