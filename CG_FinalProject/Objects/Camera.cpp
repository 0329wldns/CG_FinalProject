#include "pch.h"
#include "Objects/Camera.h"
#include "Managers/ShaderManager.h"
#include "Managers/TimeManager.h"
#include "Constants.h"

Camera::Camera(Vector3 _pos)
{
	setPos(_pos);
	at = _pos;
	at.z = 0.0f;
	oldAt = at;
}

Camera::Camera(Vector3 _pos, Vector3 _at)
	: at(_at)
	, oldAt(_at)
{
	setPos(_pos);
}

Camera::Camera(Vector3 _pos, Vector3 _at, float _fovy)
	: at(_at)
	, oldAt(_at)
	, fovy(_fovy)
{
	setPos(_pos);
}

Camera::Camera(Vector3 _pos, Vector3 _at, Vector3 _up)
	: at(_at)
	, oldAt(_at)
, up(_up)
{
	setPos(_pos);
}

Camera::Camera(Vector3 _pos, Vector3 _at, Vector3 _up, bool _isOrthographic)
	: at(_at)
	, oldAt(_at)
	, up(_up)
	, isOrthographic(_isOrthographic)
{
	setPos(_pos);
}

void Camera::update()
{
	//탑뷰모드 부분
	auto& shader = ShaderManager::getInstance();
	shader.useProgram("default");

	const float aspect = static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT;

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

	viewMatrix = glm::lookAt(myPos, presentAt, up);
	if (curCamera == this)
	{
		if (isOrthographic)
		{
			const float halfHeight = 80.0f;
			const float halfWidth = halfHeight * aspect;

			const Matrix4 projection = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, 0.1f, VISIBLE_RANGE);

			shader.setMat4("default", "projection", projection);
			shader.setMat4("default", "view", viewMatrix);
		}
		else
		{
			shader.setMat4("default", "projection", glm::perspective(glm::radians(fovy), aspect, 0.1f, VISIBLE_RANGE));
			shader.setMat4("default", "view", viewMatrix);
		}
	}
}

void Camera::render() const
{
}

void Camera::setAt(Vector3 _at)
{
	Vector3 oldAt{ at };
	at = _at;
}
