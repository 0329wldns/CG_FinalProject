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

}

void Park::update()
{
}
