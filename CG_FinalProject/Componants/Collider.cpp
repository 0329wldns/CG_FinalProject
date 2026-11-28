#include "pch.h"
#include "Componants/Collider.h"
#include "Managers/KeyManager.h"
#include "Managers/SceneManager.h"
#include "Managers/ShaderManager.h"
#include "Managers/MeshManager.h"

static unsigned int g_Id{ 0 };

Collider::Collider()
	: owner(nullptr)
	, pos()
	, scale()
	, offset()
	, id(g_Id++)
	, isCol(false)
{
}

void Collider::finalUpdate()
{
	pos = owner->getPos() + offset;
}

void Collider::render()
{
    glm::vec3 color{ 0.0f, 1.0f, 0.0f }; // 기본 초록색 와이어프레임
    if (isCol) color = glm::vec3{ 1.0f, 0.0f, 0.0f };

    ShaderManager::getInstance().useProgram("default");

    glm::mat4 model{ 1.0f };
    if (useModelMatrix)
    {
        model = modelMatrix;
    }
    else
    {
        model = glm::translate(model, getPos());
        model = glm::scale(model, getScale());
    }

    ShaderManager::getInstance().setMat4("default", "model", model);
    ShaderManager::getInstance().setVec3("default", "color", color);
    ShaderManager::getInstance().setFloat("default", "transparency", 1.0f);

    MeshManager::getInstance().drawMesh("cube", true);
}

void Collider::onCollision(Collider* other)
{
	owner->onCollision(other);
	isCol = true;
}

void Collider::onCollisionEnter(Collider* other)
{
	owner->onCollisionEnter(other);
	isCol = true;
}

void Collider::onCollisionExit(Collider* other)
{
	owner->onCollisionExit(other);
	isCol = false;
}
