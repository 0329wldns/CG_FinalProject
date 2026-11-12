#include "pch.h"
#include "Park.h"
#include "MeshManager.h"
#include "ShaderManager.h"

Park::Park()
{

}

void Park::render() const
{
    Matrix4 model{ 1.0f };
    model = glm::translate(model, Vector3{ 0.0f, 0.0f, 0.0f });
    model = glm::scale(model, Vector3{ 10000.0f, 1.0f, 12000.0f });

    auto& shader = ShaderManager::getInstance();
    shader.setMat4("default", "model", model);
    shader.setVec3("default", "color", Vector3{ 0.15f, 0.55f, 0.20f });
    shader.setFloat("default", "transparency", 1.0f);

    MeshManager::getInstance().drawMesh("cube");
}

void Park::update()
{
}
