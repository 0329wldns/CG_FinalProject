#include "pch.h"
#include "Ball.h"
#include "MeshManager.h"
#include "ShaderManager.h"
#include "TimeManager.h"
#include "Constants.h"
#include "EventFunc.h"
#include "Scene_Start.h"
#include "SceneManager.h"

Ball::Ball(float _speed, Vector3 _destination, PITCH_TYPE _pitchType)
	: speed(_speed)
	, destination(_destination)
	, pitchType(_pitchType)
{
	startPos = Vector3(-30.0f, 170.0f, -1800.0f);

	setPos(startPos);
	setScale(Vector3{ 3.5f, 3.5f, 3.5f });

	switch (pitchType)
	{
	case PITCH_TYPE::STRAIGHT:
		break;
	case PITCH_TYPE::CURVE:
		// 커브볼 - n% 지접을 2번째 컨트롤 포인트로 설정
		midControlPoint = (startPos + destination) * 0.3f;
		midControlPoint.y = startPos.y;
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
	setScale(Vector3{ 7.0f, 7.0f, 7.0f });

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
	}

	// 18m를 1초 만에 가려면 64.8km/h
	t += dt * speed / 64.8f;

	if (t > 1.0f)
	{
		deleteObject(this);
		cout << "날아간 거리: " << glm::distance(Vector2(startPos.x, startPos.z), Vector2(destination.x, destination.z)) / 100.0f << "m" << endl;
		cout << "걸린시간: " << accTime << "초" << endl;
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

	shadowModel = glm::translate(shadowModel, Vector3{ getPos().x, 0.0f, getPos().z });
	shadowModel = glm::rotate(shadowModel, glm::radians(-90.0f), Vector3{ 1.0f, 0.0f, 0.0f });
	shadowModel = glm::scale(shadowModel, getScale());

	ShaderManager::getInstance().setMat4("default", "model", shadowModel);
	ShaderManager::getInstance().setFloat("default", "transparency", 0.7f);
	ShaderManager::getInstance().setVec3("default", "color", COLOR_BLACK);

	MeshManager::getInstance().drawCircle();
}
