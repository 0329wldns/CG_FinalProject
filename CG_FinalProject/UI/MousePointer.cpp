#include "pch.h"
#include "UI/MousePointer.h"
#include "Managers/ShaderManager.h"
#include "Managers/KeyManager.h"
#include "Managers/TimeManager.h"
#include "Managers/MeshManager.h"
#include "Constants.h"
#include "EventFunc.h"

MousePointer::MousePointer(Vector2 _pos)
	: transparency{ 1.0f }
{
	setPos(_pos);

	cout << _pos.x << ", " << _pos.y << endl;
}

void MousePointer::update()
{
	float dt{ TimeManager::getInstance().getDeltaTime() };

	transparency -= dt * 4.0f;

	if (transparency < 0.0f)
	{
		deleteObject(this);
	}
}

void MousePointer::render() const
{
	ShaderManager& shaderManager{ ShaderManager::getInstance() };

	Matrix4 model{ 1.0f };

	model = glm::translate(model, glm::vec3(getPos().x, getPos().y, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 10.0f, 1.0f));

	shaderManager.setMat4("UI", "model", model);
	shaderManager.setVec3("UI", "color", COLOR_RED);
	shaderManager.setFloat("UI", "transparency", transparency);

	MeshManager::getInstance().drawCircle();
}
