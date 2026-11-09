#include "pch.h"
#include "Camera.h"

Camera::Camera(Vector3 _pos)
{
	at = _pos;
	at.z = 0.0f;
}

Camera::Camera(Vector3 _pos, Vector3 _at)
{
}

void Camera::update()
{
}

void Camera::render() const
{
}
