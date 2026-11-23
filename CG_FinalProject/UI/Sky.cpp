#include "pch.h"
#include "UI/Sky.h"
#include "Objects/Camera.h"
#include "Managers/ShaderManager.h"
#include "Managers/MeshManager.h"
#include "Constants.h"

void Sky::update()
{
}

void Sky::render() const
{
	const Camera* camera = Camera::getCurCamera();
	if (!camera)
		return;

	auto& shader = ShaderManager::getInstance();
	shader.useProgram("sky");
	shader.setMat4("sky", "inverseView", glm::inverse(camera->getViewMatrix()));
	shader.setFloat("sky", "tanHalfFovy",
		glm::tan(glm::radians(camera->getFovy() * 0.5f)));
	shader.setFloat("sky", "aspect", static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT);
	shader.setBool("sky", "orthographic", camera->getIsOrthographic());

	// 배경은 깊이 검사와 기록 없이 먼저 그린다.
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	MeshManager::getInstance().drawQuad();

	// 뒤이어 그리는 그리드와 구장의 상태 복구
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	shader.useProgram("default");
}
