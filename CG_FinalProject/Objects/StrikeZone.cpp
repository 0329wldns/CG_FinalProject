#include "pch.h"
#include "StrikeZone.h"
#include "Constants.h"
#include "ShaderManager.h"
#include "MeshManager.h"

StrikeZone::StrikeZone()
{
	setPos(Vector3(STRIKE_ZONE::CENTER));
	setScale(Vector3(STRIKE_ZONE::WIDTH, STRIKE_ZONE::HEIGHT, STRIKE_ZONE::DEPTH));
}

void StrikeZone::update()
{
}

void StrikeZone::render() const
{
	Matrix4 model{ 1.0f };

	model = glm::translate(model, getPos());
	model = glm::scale(model, getScale());

	ShaderManager::getInstance().setMat4("default", "model", model);
	ShaderManager::getInstance().setFloat("default", "transparency", 1.0f);
	ShaderManager::getInstance().setVec3("default", "color", COLOR_BLACK);

	MeshManager::getInstance().drawMesh("cube", true);
}
