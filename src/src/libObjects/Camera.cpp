#include "Camera.h"

Camera::Camera()
:Object()
{
}

Camera::Camera(Vector3f pos, Vector3f up, Vector3f target)
:Object()
{
	m_Up = up;
	m_Target = target;
}
