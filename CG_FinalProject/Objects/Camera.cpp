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

Camera::Camera(Vector3 _pos, Vector3 _at, Vector3 _up)
	: at(_at)
	, oldAt(_at)
, up(_up)
{
	setPos(_pos);
}

void Camera::update()
{
	//탑뷰모드 부분
	auto& shader = ShaderManager::getInstance();
	shader.useProgram("default");

	const float aspect = static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT;

	if (topView)
	{
		// 현재 잔디의 깊이 12000에 여백을 더해 위아래로 12600을 표시한다.
		const float halfHeight = 6300.0f;
		const float halfWidth = halfHeight * aspect;

		const Matrix4 projection = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, 0.1f, VISIBLE_RANGE);

		// 바로 위에서 내려다보고, 외야(-Z)가 화면 위쪽을 향하게 한다.
		const Matrix4 view = glm::lookAt(Vector3{ 0.0f, 5000.0f, 0.0f },Vector3{ 0.0f, 0.0f, 0.0f },Vector3{ 0.0f,0.0f, -1.0f });

		shader.setMat4("default", "projection", projection);
		shader.setMat4("default", "view", view);
		return;
	}

	shader.setMat4("default", "projection",glm::perspective(glm::radians(60.0f), aspect, 0.1f, VISIBLE_RANGE));

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
		ShaderManager::getInstance().useProgram("default");
		ShaderManager::getInstance().setMat4("default", "view", viewMatrix);
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
