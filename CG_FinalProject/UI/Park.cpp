#include "pch.h"
#include "UI/Park.h"
#include "Managers/MeshManager.h"
#include "Managers/ShaderManager.h"

Park::Park()
{

}

void Park::render() const
{
	//잔디 평면 추가 
	Matrix4 model{ 1.0f };
	model = glm::translate(model, Vector3{ 0.0f, 0.0f, 0.0f });
	model = glm::scale(model, Vector3{ 10000.0f, 1.0f, 12000.0f });

	auto& shader = ShaderManager::getInstance();
	shader.setMat4("default", "model", model);
	shader.setVec3("default", "color", Vector3{ 0.15f, 0.55f, 0.20f });
	shader.setFloat("default", "transparency", 1.0f);

	MeshManager::getInstance().drawMesh("cube");

	// 홈플레이트와 양쪽 타자석을 감싸는 흙 바닥
	Matrix4 dirtModel = glm::translate(Matrix4{ 1.0f }, Vector3{ 0.0f, 0.6f, -40.0f });
	dirtModel = glm::scale(dirtModel, Vector3{ 150.0f, 1.0f, 150.0f });
	shader.setMat4("default", "model", dirtModel);
	shader.setVec3("default", "color", Vector3{ 0.55f, 0.32f, 0.17f });
	MeshManager::getInstance().drawMesh("home_dirt");

	// 투수 마운드의 흙 영역, 투수 마운드는 기본적으로 경사져있음
	Matrix4 moundModel = glm::translate(Matrix4{ 1.0f }, Vector3{ 0.0f, 0.6f, -1800.0f });

	shader.setMat4("default", "model", moundModel);
	shader.setVec3("default", "color", Vector3{ 0.55f, 0.32f, 0.17f });
	MeshManager::getInstance().drawMesh("pitcher_mound");

	// 투수판 밑면을 마운드 윗면에 맞춘다.
	Matrix4 moundBaseModel = glm::translate(Matrix4{ 1.0f }, Vector3{ 0.0f, 27.5f, -1800.0f });
	moundBaseModel = glm::scale(moundBaseModel, Vector3{ 61.0f, 4.0f, 15.2f });

	shader.setMat4("default", "model", moundBaseModel);
	shader.setVec3("default", "color", Vector3{ 1.0f, 1.0f, 1.0f });
	MeshManager::getInstance().drawMesh("cube");

	//홈플레이트 추가
	Matrix4 homePlateModel{ 1.0f };

	shader.setMat4("default", "model", homePlateModel);
	shader.setVec3("default", "color", Vector3{ 1.0f, 1.0f, 1.0f });
	MeshManager::getInstance().drawMesh("home_plate");

	//배터 박스 추가
	auto drawBatterBoxLine = [&shader](const Vector3& position, const Vector3& scale) // 람다에서 행렬 제작
		{
			Matrix4 model{ 1.0f };
			model = glm::translate(model, position);
			model = glm::scale(model, scale);

			shader.setMat4("default", "model", model);
			MeshManager::getInstance().drawMesh("cube");
		};

	shader.setVec3("default", "color", Vector3{ 1.0f, 1.0f, 1.0f });

	// X 좌표의 부호를 바꿔 홈플레이트 양쪽에 타자석을 그린다.
	for (const float side : { 1.0f, -1.0f })
	{
		// 세로선 두 개
		drawBatterBoxLine({ side * 37.0f, 1.0f, -40.0f }, { 2.0f, 1.0f, 120.0f });
		drawBatterBoxLine({ side * 117.0f, 1.0f, -40.0f }, { 2.0f, 1.0f, 120.0f });

		// 가로선 두 개
		drawBatterBoxLine({ side * 77.0f, 1.0f, -100.0f }, { 80.0f, 1.0f, 2.0f });
		drawBatterBoxLine({ side * 77.0f, 1.0f,   20.0f }, { 80.0f, 1.0f, 2.0f });
	}

	//파울라인 추가
	const Vector3 start{ 102.0f, 1.0f, -102.0f };
	const Vector3 end{ 1939.7f, 1.0f, -1939.7f };

	for (const float side : { 1.0f, -1.0f }) // 오른쪽과 왼쪽 파울라인을 그리기 위해 side를 1.0f와 -1.0f로 설정
	{
		Vector3 center = (start + end) * 0.5f; //파울라인으로 사용할 큐브의 중심을 선의 시작점과 끝점 사이
		center.x *= side; // side가 -1.0f이면 x좌표를 반전시켜 왼쪽 파울라인을 그리도록 함

		Matrix4 foulLineModel{ 1.0f }; // 각 선의 변환은 단위 행렬 초기화
		foulLineModel = glm::translate(foulLineModel, center);
		// 파울라인을 회전시키기 위해 -45도 * side만큼 회전, side가 -1.0f이면 왼쪽 파울라인이므로 반대 방향으로 회전
		foulLineModel = glm::rotate(foulLineModel, glm::radians(-45.0f * side), Vector3{ 0.0f, 1.0f, 0.0f });
		foulLineModel = glm::scale(foulLineModel, Vector3{ 2.0f, 1.0f, glm::distance(start, end) });

		shader.setMat4("default", "model", foulLineModel);
		MeshManager::getInstance().drawMesh("cube");
	}

	// 1루와 3루 추가
	const Vector3 baseScale{ 70.0f, 14.0f, 70.0f };
	const float cornerOffset = baseScale.x / glm::sqrt(2.0f);
	for (const float side : { 1.0f, -1.0f })
	{
		Matrix4 baseModel{ 1.0f };
		baseModel = glm::translate(baseModel, Vector3{ side * end.x, 7.5f, end.z - cornerOffset });
		baseModel = glm::rotate(baseModel, glm::radians(45.0f), Vector3{ 0.0f, 1.0f, 0.0f });
		baseModel = glm::scale(baseModel, baseScale);

		shader.setMat4("default", "model", baseModel);
		MeshManager::getInstance().drawMesh("cube");
	}

	// 2루 추가

	// 2루 베이스
	const Vector3 secondBasePos{ 0.0f, 7.5f, end.z * 2.0f };
	const Vector3 up{ 0.0f, 1.0f, 0.0f };
	auto& meshes = MeshManager::getInstance();

	Matrix4 secondBaseModel = glm::translate(Matrix4{ 1.0f }, secondBasePos);
	secondBaseModel = glm::rotate(secondBaseModel, glm::radians(45.0f), up);
	secondBaseModel = glm::scale(secondBaseModel, Vector3{ 70.0f, 14.0f, 70.0f });

	shader.setMat4("default", "model", secondBaseModel);
	meshes.drawMesh("cube");

	// 1루와 3루에서 각각 2루로 이어지는 확인용 선
	const Vector3 to{ secondBasePos.x, 4.5f, secondBasePos.z };

	for (const float side : { 1.0f, -1.0f })
	{
		const Vector3 from{ side * end.x, 4.5f, end.z - cornerOffset };
		const Vector3 direction = to - from;

		Matrix4 lineModel = glm::translate(Matrix4{ 1.0f }, (from + to) * 0.5f);
		lineModel = glm::rotate(lineModel, glm::atan(direction.x, direction.z), up);
		lineModel = glm::scale(lineModel, Vector3{ 16.0f, 8.0f, glm::length(direction) });

		shader.setMat4("default", "model", lineModel);
		meshes.drawMesh("cube");
	}

}

void Park::update()
{
}
