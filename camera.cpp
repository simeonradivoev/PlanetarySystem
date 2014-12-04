#include "camera.h"

Camera* Camera::m_currentCamera = nullptr;

void Camera::Bind()
{
	Camera::m_currentCamera = this;
}

Camera::~Camera()
{
}
