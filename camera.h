#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "transform.h"
#include "display.h"
#include "glm\glm.hpp"
#include "GLFW\glfw3.h"
#include "physics.h"
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

	void Bind();
	inline static Camera* GetCurrentCamera(){ return Camera::m_currentCamera; }

	inline dmat4 GetViewProjection()
	{
		return GetProjection() * GetViewMatrix();
	}

	inline dmat4 GetProjection()
	{
		return glm::perspective(m_fov, (double)Display::GetCurrentDisplay()->GetWidth() / (double)Display::GetCurrentDisplay()->GetHeight(), m_zNear, m_ZFar);
	}

	inline dmat4 GetViewMatrix()
	{
		return mat4_cast(-transform.rotation) *glm::translate(-transform.position);
	}

	inline Physics::Ray ScreenToRay(glm::vec2 screenPos)
	{
		glm::dvec3 cameraPosition = transform.GetPos();
		glm::dmat4 inverseViewProjection = glm::inverse(GetViewProjection());
		double screenWidth = Display::GetCurrentDisplay()->GetWidth();
		double screenHeight = Display::GetCurrentDisplay()->GetHeight();
		double x = (2.0 * (screenPos.x / screenWidth)) - 1.0;
		double y = 1.0 - (2.0 * (screenPos.y / screenHeight));
		glm::dvec4 pos(x,y,m_zNear,1.0);
		pos = inverseViewProjection * pos;
		glm::dvec3 fpos = (glm::dvec3(pos) / pos.w);
		glm::dvec3 dir = (fpos - cameraPosition);
		return Physics::Ray(fpos, dir);
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
	static Camera* m_currentCamera;
};
#endif	//CAMERA_H
