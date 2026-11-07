#include "pch.h"
#include "Ball.h"
#include "MeshManager.h"
#include "ShaderManager.h"
#include "TimeManager.h"
#include "Constants.h"
#include "EventFunc.h"

Ball::Ball(float _speed)
	: speed( _speed )
{
	setPos(Vector3{ 0.0f, 0.0f, -1800.0f });
	setScale(Vector3{ 7.0f, 7.0f, 7.0f });
}

Ball::~Ball()
{
	cout << "Ball deleted" << endl;
}

void Ball::update()
{
	Vector3 myPos{ getPos() };
	float dt{ TimeManager::getInstance().getDeltaTime() };

	// km/h 단위 속력을 cm/s 단위로 변환 후 이동
	float speed_cps{ speed / 0.036f };

	myPos += direction * speed_cps * dt;
	if (myPos.z > 0.0f)
	{
		deleteObject(this);
	}

	setPos( myPos );
}

void Ball::render() const
{
	Matrix4 model{ 1.0f };

	model = glm::translate(model, getPos());
	model = glm::scale(model, getScale());

	ShaderManager::getInstance().setMat4("default", "model", model);
	ShaderManager::getInstance().setFloat("default", "transparency", 1.0f);
	ShaderManager::getInstance().setVec3("default", "objectColor", COLOR_BLACK);

	MeshManager::getInstance().drawMesh("sphere");
}
