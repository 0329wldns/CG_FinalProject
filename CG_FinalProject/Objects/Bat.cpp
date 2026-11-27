#include "pch.h"
#include "Objects/Bat.h"
#include "Managers/MeshManager.h"
#include "Managers/ShaderManager.h"

void Bat::setPose(const Matrix4& transform, bool isSwinging)
{
    model = transform;
    setPos(Vector3(model[3]));
    swinging = isSwinging;
}

void Bat::render() const
{
    auto& shader = ShaderManager::getInstance();
    shader.setMat4("default", "model", model);
    shader.setVec3("default", "color", Vector3{ 1.0f });
    shader.setFloat("default", "transparency", 1.0f);
    shader.setBool("default", "useTexture", false);
    MeshManager::getInstance().drawMesh("baseball_bat");
}
