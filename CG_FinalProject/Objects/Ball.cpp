#include "pch.h"
#include "Ball.h"
#include "MeshManager.h"
#include "ShaderManager.h"
#include "TimeManager.h"
#include "Constants.h"
#include "EventFunc.h"

Ball::Ball(float _speed, Vector3 _destination, PITCH_TYPE _pitchType)
	: speed(_speed)
	, destination(_destination)
	, pitchType(_pitchType)
{
	startPos = Vector3(0.0f, 150.0f, -1800.0f);

	setPos(startPos);
	setScale(Vector3{ 7.0f, 7.0f, 7.0f });

	switch (pitchType)
	{
	case PITCH_TYPE::STRAIGHT:
		// 직구 - 컨트롤 포인트가 직선위에 있음
		midControlPoint = (startPos + destination) * 0.5f;
		break;
	case PITCH_TYPE::CURVE:
		// 커브볼 - n% 지접을 2번째 컨트롤 포인트로 설정
		midControlPoint = (startPos + destination) * 0.7f;
		midControlPoint.y = startPos.y + 100.0f;
		break;
	}

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

	Vector3 myPos{ (1.0f - t) * (1.0f - t) * startPos
			+ 2.0f * (1.0f - t) * t * midControlPoint
			+ t * t * destination };

	t += dt;
	if (t > 1.0f)
	{
		deleteObject(this);
		cout << "공 날아간 거리: " << glm::distance(Vector2(startPos.x, startPos.z), Vector2(destination.x, destination.z)) / 100.0f << "m" << endl;
	}

	setPos( myPos );
}

void Ball::render() const
{
	// 공 궤적 렌더링
	glDisable(GL_DEPTH_TEST);
	ShaderManager::getInstance().setMat4("default", "model", Matrix4{ 1.0f });
	glPointSize(4.0f);
	glBindVertexArray(trajectoryVAO);
	glDrawArrays(GL_POINTS, 0, 100);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);

	// 공 렌더링
	Matrix4 ballModel{ 1.0f };

	ballModel = glm::translate(ballModel, getPos());
	ballModel = glm::scale(ballModel, getScale());

	ShaderManager::getInstance().setMat4("default", "model", ballModel);
	ShaderManager::getInstance().setFloat("default", "transparency", 1.0f);
	ShaderManager::getInstance().setVec3("default", "color", COLOR_RED);

	MeshManager::getInstance().drawMesh("sphere");
}
