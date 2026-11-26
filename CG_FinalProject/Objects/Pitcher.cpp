#include "pch.h"
#include "Objects/Pitcher.h"
#include "Constants.h"
#include "Managers/KeyManager.h"
#include "Managers/SceneManager.h"
#include "Scenes/Scene_Start.h"
#include "UI/MousePointer.h"
#include "Eventfunc.h"
#include "Managers/ShaderManager.h"

Pitcher::Pitcher()
{
	setPos(Vector3(0.0f, 0.0f, MOUND_Z));
}

void Pitcher::update()
{
	static PITCH_TYPE pitchType{ PITCH_TYPE::STRAIGHT };
	static const Vector3 startPos = Vector3(0.0f, 1.7f, MOUND_Z);
	static Vector3 destination{ 0.0f, 0.0f, 0.0f };
	static Vector3 midPos{ 0.0f, 0.0f, 0.0f };

	Scene_Start* curScene{ static_cast<Scene_Start*>(SceneManager::getInstance().getCurScene()) };
	KeyManager& keyManager{ KeyManager::getInstance() };

	Vector3 worldMousePos{ curScene->getWorldMousePos() };

	// 던질 구종 고르기
	if (keyManager.getKeyState(KEY::NUM1) == KEY_STATE::TAP)
	{
		pitchType = PITCH_TYPE::STRAIGHT;
	}
	if (keyManager.getKeyState(KEY::NUM2) == KEY_STATE::TAP)
	{
		pitchType = PITCH_TYPE::CURVE;
	}

	if (keyManager.getKeyState(KEY::MOUSE_L) == KEY_STATE::TAP)
	{
		Vector2 mousePos{ keyManager.getMousePos() };
		MousePointer* mousePointer = new MousePointer(mousePos);
		createUI(mousePointer, UI_GROUP::MOUSE_POINTER);

		destination = worldMousePos ;
	}
	if (keyManager.getKeyState(KEY::MOUSE_L) == KEY_STATE::HOLD)
	{
		midPos = worldMousePos;
		midPos.z = MOUND_Z * 0.5f;

		Vector3 midControlPoint = (midPos - ((1.0f - 0.5f) * (1.0f - 0.5f) * startPos
			+ 0.5f * 0.5f * destination)) / (2.0f * 0.5f * (1.0f - 0.5f));

		std::vector<std::array<float, 3>> trajectoryVertices;
		trajectoryVertices.reserve(50);

		for (float t = 0.0f; t <= 1.0f; t += 0.02f)
		{
			Vector3 pos{ (1.0f - t) * (1.0f - t) * startPos
				+ 2.0f * (1.0f - t) * t * midControlPoint
				+ t * t * destination };

			trajectoryVertices.push_back({ pos.x, pos.y, pos.z });
		}

		glGenVertexArrays(1, &trajectoryVAO);
		glGenBuffers(1, &trajectoryVBO);

		glBindVertexArray(trajectoryVAO);
		glBindBuffer(GL_ARRAY_BUFFER, trajectoryVBO);
		glBufferData(GL_ARRAY_BUFFER, trajectoryVertices.size() * sizeof(std::array<float, 3>), trajectoryVertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	if (keyManager.getKeyState(KEY::MOUSE_L) == KEY_STATE::AWAY)
	{
		glDeleteVertexArrays(1, &trajectoryVAO);
		glDeleteBuffers(1, &trajectoryVBO);

		Ball* ball = new Ball(10.0f, destination, midPos, pitchType);
		createObject(ball, OBJECT_GROUP::BALL);
	}
}

void Pitcher::render() const
{
	if (KeyManager::getInstance().getKeyState(KEY::MOUSE_L) == KEY_STATE::HOLD)
	{
		glDisable(GL_DEPTH_TEST);
		ShaderManager::getInstance().setMat4("default", "model", Matrix4{ 1.0f });
		ShaderManager::getInstance().setFloat("default", "transparency", 0.5f);
		glPointSize(3.0f);
		glBindVertexArray(trajectoryVAO);
		glDrawArrays(GL_POINTS, 0, 100);
		glBindVertexArray(0);
		glEnable(GL_DEPTH_TEST);
	}
}
