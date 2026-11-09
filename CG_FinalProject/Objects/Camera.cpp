#include "pch.h"
#include "Camera.h"
#include "ShaderManager.h"
#include "TimeManager.h"

Camera::Camera(Vector3 _pos)
	: t(0.0f)
{
	at = _pos;
	at.z = 0.0f;
	oldAt = at;
}

Camera::Camera(Vector3 _pos, Vector3 _at)
	: t(0.0f)
{
	setPos(_pos);
	at = _at;
	oldAt = at;
}

void Camera::update()
{
	float dt{ TimeManager::getInstance().getDeltaTime() };
	Vector3 myPos{ getPos() };
	
	if (oldAt != at)
	{
		t += dt * 5.0f;
	}

	Vector3 presentAt{ (1.0f - t) * oldAt + t * at };

	if (t >= 1.0f)
	{
		t = 0.0f;
		oldAt = at;
	}

	glm::mat4 view{
		glm::lookAt(myPos
		, presentAt
		, glm::vec3(0.0f, 1.0f, 0.0f))
	};
	ShaderManager::getInstance().useProgram("default");
	ShaderManager::getInstance().setMat4("default", "view", view);
}

void Camera::render() const
{
}

void Camera::setAt(Vector3 _at)
{
	Vector3 oldAt{ at };
	at = _at;
}
