#include "pch.h"
#include "Objects/Ball.h"
#include "Managers/MeshManager.h"
#include "Managers/ShaderManager.h"
#include "Managers/TimeManager.h"
#include "Constants.h"
#include "EventFunc.h"
#include "Scenes/Scene_Start.h"
#include "Managers/SceneManager.h"

Ball::Ball(float _speed, Vector3 _destination, Vector3 _midPos, PITCH_TYPE _pitchType)
	: speed(_speed)
	, destination(_destination)
	, pitchType(_pitchType)
{
	startPos = Vector3(0.0f, 1.7f, MOUND_Z);

	setPos(startPos);
	setScale(Vector3{ 0.0365f, 0.0365f, 0.0365f });

	switch (pitchType)
	{
	case PITCH_TYPE::STRAIGHT:
		break;
	case PITCH_TYPE::CURVE:
		midControlPoint = (_midPos - ((1.0f - 0.5f) * (1.0f - 0.5f) * startPos
			+ 0.5f * 0.5f * destination)) / (2.0f * 0.5f * (1.0f - 0.5f));
		midControlPoint.z = MOUND_Z * 0.5f;

		break;
	}

	std::vector<std::array<float, 3>> trajectoryVertices;
	trajectoryVertices.reserve(50);
	if (pitchType == PITCH_TYPE::STRAIGHT)
	{
		for (float t = 0.0f; t <= 1.0f; t += 0.02f)
		{
			Vector3 pos{ (1.0f - t) * startPos + t * destination };

			trajectoryVertices.push_back({ pos.x, pos.y, pos.z });
		}
	}
	else
	{
		for (float t = 0.0f; t <= 1.0f; t += 0.02f)
		{
			Vector3 pos{ (1.0f - t) * (1.0f - t) * startPos
				+ 2.0f * (1.0f - t) * t * midControlPoint
				+ t * t * destination };

			trajectoryVertices.push_back({ pos.x, pos.y, pos.z });
		}
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

Ball::Ball(float _speed, Vector3 _destination)
	: speed( _speed )
	, destination( _destination )
	, startPos( Vector3{ 0.0f, 0.0f, 0.0f } )
{
	setPos(startPos);
	setScale(Vector3{ 0.07f, 0.07f, 0.07f });

	// 공 궤적 정점 버퍼 세팅
	std::vector<std::array<float, 3>> trajectoryVertices;
	trajectoryVertices.reserve(100);

	for (float t = 0.0f; t <= 1.0f; t += 0.01f)
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

Ball::~Ball()
{
	cout << "날아간 거리: " << glm::distance(Vector2(startPos.x, startPos.z), Vector2(destination.x, destination.z)) << "m" << endl;
	cout << "걸린시간: " << accTime << "초" << endl;
	cout << "Ball deleted" << endl;
}

void Ball::update()
{
	float dt{ TimeManager::getInstance().getDeltaTime() };

	accTime += dt;

	Vector3 myPos{};

	if (pitchType == PITCH_TYPE::STRAIGHT)
	{
		myPos = (1.0f - t) * startPos + t * destination;
	}
	else
	{
		myPos = (1.0f - t) * (1.0f - t) * startPos
			+ 2.0f * (1.0f - t) * t * midControlPoint
			+ t * t * destination;

		direction = 2.0f * (1.0f - t) * (midControlPoint - startPos)
			+ 2.0f * t * (destination - midControlPoint);
		direction = glm::normalize(direction);

		cout << "direction: " << direction.x << ", " << direction.y << ", " << direction.z << endl;
	}

	t += dt * speed / 66.384f;

	if (t > 1.0f)
	{
		deleteObject(this);
	}

	setPos( myPos );
}

void Ball::render() const
{
	Scene_Start* curScene{ static_cast<Scene_Start*>(SceneManager::getInstance().getCurScene()) };

	// 공 궤적 렌더링
	if (curScene->isShowTrajectory())
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

	// 공 렌더링
	Matrix4 ballModel{ 1.0f };

	ballModel = glm::translate(ballModel, getPos());
	ballModel = glm::scale(ballModel, getScale());

	ShaderManager::getInstance().setMat4("default", "model", ballModel);
	ShaderManager::getInstance().setFloat("default", "transparency", 1.0f);
	ShaderManager::getInstance().setVec3("default", "color", COLOR_RED);

	MeshManager::getInstance().drawMesh("sphere");

	// 공 그림자 렌더링
	Matrix4 shadowModel{ 1.0f };

	shadowModel = glm::translate(shadowModel, Vector3{ getPos().x, 0.01f, getPos().z });
	shadowModel = glm::rotate(shadowModel, glm::radians(-90.0f), Vector3{ 1.0f, 0.0f, 0.0f });
	shadowModel = glm::scale(shadowModel, getScale());

	ShaderManager::getInstance().setMat4("default", "model", shadowModel);
	ShaderManager::getInstance().setFloat("default", "transparency", 0.5f);
	ShaderManager::getInstance().setVec3("default", "color", COLOR_BLACK);

	MeshManager::getInstance().drawCircle();

	// 공 방향벡터 렌더링
	Matrix4 directionModel{ 1.0f };

	glm::vec3 fromDir{ 1.0f, 0.0f, 0.0f };

	float cosTheta{ glm::clamp(glm::dot(fromDir, direction), -1.0f, 1.0f) };
	float angleRad{ std::acos(cosTheta) };
	glm::vec3 axis{ glm::cross(fromDir, direction) };

	directionModel = glm::translate(directionModel, getPos());
	if (glm::length(axis) > 1e-6f)
	{
		directionModel = glm::rotate(directionModel, angleRad, glm::normalize(axis));
	}
	directionModel = glm::scale(directionModel, Vector3{ 1.0f, 1.0f, 1.0f });

	ShaderManager::getInstance().setMat4("default", "model", directionModel);
	ShaderManager::getInstance().setFloat("default", "transparency", 1.0f);
	ShaderManager::getInstance().setVec3("default", "color", COLOR_BLUE);

	MeshManager::getInstance().drawLine();
}
