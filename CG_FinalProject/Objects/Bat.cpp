#include "pch.h"
#include "Objects/Bat.h"
#include "Managers/MeshManager.h"
#include "Managers/ShaderManager.h"

Bat::Bat()
{
	createCollider();
}

void Bat::setPose(const Matrix4& transform, bool isSwinging)
{
	model = transform;
	setPos(Vector3(model[3]));
	swinging = isSwinging;

	Collider* col = getCollider();
	if (col)
	{
		Vector3 boxScale{ getBarrelTip() - getBarrelStart() };

		// 손잡이 기준 Y축으로 절반(0.39m) 올라간 지점이 박스 중심
		Vector3 boxCenterOffset{ 0.0f, 0.78f * 0.7f, 0.0f };

		// 배트의 회전/위치 행렬에 오프셋과 스케일 적용
		Matrix4 colModel = model;
		colModel = glm::translate(colModel, boxCenterOffset);
		colModel = glm::scale(colModel, boxScale);
		col->setModelMatrix(colModel);
	}
}

void Bat::render() const
{
	auto& shader = ShaderManager::getInstance();
	shader.setMat4("default", "model", model);
	shader.setVec3("default", "color", Vector3{ 1.0f });
	shader.setFloat("default", "transparency", 1.0f);
	shader.setBool("default", "useTexture", false);
	MeshManager::getInstance().drawMesh("baseball_bat");

	componantRender();
}
