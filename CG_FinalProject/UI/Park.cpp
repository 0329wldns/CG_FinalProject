#include "pch.h"
#include "UI/Park.h"
#include "Constants.h"
#include "Managers/MeshManager.h"
#include "Managers/ShaderManager.h"
#include "Managers/TextureManager.h"

Park::Park()
{

}

void Park::render() const
{
	//잔디 평면 추가 
	Matrix4 model{ 1.0f };
	model = glm::translate(model, Vector3{ 0.0f, 0.0f, 0.0f });
	model = glm::scale(model, Vector3{ 300.0f, 0.01f, 240.0f });

	auto& shader = ShaderManager::getInstance();
	shader.setMat4("default", "model", model);
	shader.setVec3("default", "color", Vector3{ 0.15f, 0.55f, 0.20f });
	shader.setFloat("default", "transparency", 1.0f);

	MeshManager::getInstance().drawMesh("cube");

	// 홈플레이트와 양쪽 타자석을 감싸는 흙 바닥
	Matrix4 dirtModel = glm::translate(Matrix4{ 1.0f }, Vector3{ 0.0f, 0.006f, -0.4f });
	dirtModel = glm::scale(dirtModel, Vector3{ 1.5f, 1.0f, 1.5f });
	shader.setMat4("default", "model", dirtModel);
	shader.setVec3("default", "color", Vector3{ 0.55f, 0.32f, 0.17f });
	MeshManager::getInstance().drawMesh("home_dirt");

	// 투수 마운드의 흙 영역, 투수 마운드는 기본적으로 경사져있음
	Matrix4 moundModel = glm::translate(Matrix4{ 1.0f }, Vector3{ 0.0f, 0.006f, MOUND_Z });
	moundModel = glm::scale(moundModel, Vector3{ 0.01f, 0.01f, 0.01f });

	shader.setMat4("default", "model", moundModel);
	shader.setVec3("default", "color", Vector3{ 0.55f, 0.32f, 0.17f });
	MeshManager::getInstance().drawMesh("pitcher_mound");

	// 투수판 밑면을 마운드 윗면에 맞춘다.
	Matrix4 moundBaseModel = glm::translate(Matrix4{ 1.0f }, Vector3{ 0.0f, 0.255f, MOUND_Z });
	moundBaseModel = glm::scale(moundBaseModel, Vector3{ 0.61f, 0.04f, 0.152f });

	shader.setMat4("default", "model", moundBaseModel);
	shader.setVec3("default", "color", Vector3{ 1.0f, 1.0f, 1.0f });
	MeshManager::getInstance().drawMesh("cube");

	//홈플레이트 추가
	Matrix4 homePlateModel = glm::scale(Matrix4{ 1.0f }, Vector3{ 0.01f, 0.01f, 0.01f });

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
		drawBatterBoxLine({ side * 0.37f, 0.01f, -0.4f }, { 0.02f, 0.01f, 1.2f });
		drawBatterBoxLine({ side * 1.17f, 0.01f, -0.4f }, { 0.02f, 0.01f, 1.2f });

		// 가로선 두 개
		drawBatterBoxLine({ side * 0.77f, 0.01f, -1.0f }, { 0.8f, 0.01f, 0.02f });
		drawBatterBoxLine({ side * 0.77f, 0.01f,   0.2f }, { 0.8f, 0.01f, 0.02f });
	}

	//파울라인 추가
	const Vector3 start{ 1.02f, 0.01f, -1.02f };
	const float baseAxisOffset = 19.397f;
	const float poleAxisOffset = 70.0f;
	const Vector3 baseEnd{ baseAxisOffset, 0.01f, -baseAxisOffset };
	const Vector3 foulLineEnd{ poleAxisOffset, 0.01f, -poleAxisOffset };

	for (const float side : { 1.0f, -1.0f }) // 오른쪽과 왼쪽 파울라인을 그리기 위해 side를 1.0f와 -1.0f로 설정
	{
		Vector3 center = (start + foulLineEnd) * 0.5f; // 파울라인 큐브의 중심은 시작점과 끝점의 중간
		center.x *= side; // side가 -1.0f이면 x좌표를 반전시켜 왼쪽 파울라인을 그리도록 함

		Matrix4 foulLineModel{ 1.0f }; // 각 선의 변환은 단위 행렬 초기화
		foulLineModel = glm::translate(foulLineModel, center);
		// 파울라인을 회전시키기 위해 -45도 * side만큼 회전, side가 -1.0f이면 왼쪽 파울라인이므로 반대 방향으로 회전
		foulLineModel = glm::rotate(foulLineModel, glm::radians(-45.0f * side), Vector3{ 0.0f, 1.0f, 0.0f });
		foulLineModel = glm::scale(foulLineModel, Vector3{ 0.05f, 0.01f, glm::distance(start, foulLineEnd) });

		shader.setVec3("default", "color", Vector3{ 1.0f, 1.0f, 1.0f });
		shader.setMat4("default", "model", foulLineModel);
		MeshManager::getInstance().drawMesh("cube");

		Vector3 polePosition = foulLineEnd;
		polePosition.x *= side;  // 1루·3루 방향에 맞춰 X 좌표를 반전
		polePosition.y = 6.0f;

		Matrix4 poleModel{ 1.0f };
		poleModel = glm::translate(poleModel, polePosition);
		poleModel = glm::scale(poleModel, Vector3{ 0.3f, 12.0f, 0.3f });

		shader.setMat4("default", "model", poleModel);
		shader.setVec3("default", "color", Vector3{ 1.0f, 1.0f, 0.0f });
		MeshManager::getInstance().drawMesh("cube");
	}
	shader.setVec3("default", "color", Vector3{ 1.0f, 1.0f, 1.0f });

	// 외야 펜스와 반복해서 배치할 광고 텍스처
	const char* adNames[] = { "fence_ad", "power", "burger", "ace", "coffee" }; 
	auto& textures = TextureManager::getInstance(); // 텍스처 관리자 인스턴스를 가져온다.
	const int fenceSegments = 32;
	const float centerFieldZ = -110.0f;// 외야 펜스의 중앙 Z 좌표, 펜스는 중앙에서 양쪽으로 곡선 형태로 그려짐

	// t가 -1이면 왼쪽 폴, 0이면 중앙 펜스, +1이면 오른쪽 폴
	auto fencePoint = [&](float t)
		{
			return Vector3{
				foulLineEnd.x * t,
				1.0f,
				centerFieldZ + (foulLineEnd.z - centerFieldZ) * t * t
			};
		};

	shader.setVec3("default", "color", Vector3{ 0.05f, 0.25f, 0.15f });

	for (int i = 0; i < fenceSegments; ++i) // 펜스의 각 세그먼트를 그리기 위해 fenceSegments만큼 반복
	{
		const float t = -1.0f + 2.0f * i / fenceSegments; // t가 -1이면 왼쪽 폴, 0이면 중앙 펜스, +1이면 오른쪽 폴
		const float nextT = -1.0f + 2.0f * (i + 1) / fenceSegments; // 다음 세그먼트의 t 값, 펜스는 곡선 형태이므로 각 세그먼트를 직선으로 근사하여 그린다.

		const Vector3 from = fencePoint(t); // 펜스의 시작점 좌표, t 값에 따라 왼쪽 폴, 중앙 펜스, 오른쪽 폴 중 하나를 선택
		const Vector3 to = fencePoint(nextT); // 펜스의 끝점 좌표, 다음 세그먼트의 t 값에 따라 왼쪽 폴, 중앙 펜스, 오른쪽 폴 중 하나를 선택
		const Vector3 direction = to - from;

		Matrix4 fenceModel{ 1.0f };
		fenceModel = glm::translate(fenceModel, (from + to) * 0.5f);
		fenceModel = glm::rotate(
			fenceModel, glm::atan(direction.x, direction.z),
			Vector3{ 0.0f, 1.0f, 0.0f });
		fenceModel = glm::scale(
			fenceModel,
			Vector3{ 0.2f, 2.0f, glm::length(direction) + 0.02f });

		shader.setMat4("default", "model", fenceModel);
		MeshManager::getInstance().drawMesh("cube");

		if (textures.bindTexture(adNames[i % 5])) // 광고 텍스처를 바인딩, 5개의 광고 텍스처를 반복해서 사용
		{
			const Vector3 up{ 0.0f, 1.0f, 0.0f }; // 광고 패널의 위쪽 방향 벡터, 펜스의 위쪽 방향과 동일하게 설정
			const float length = glm::length(direction); // 광고 패널의 길이, 펜스 세그먼트의 길이와 동일하게 설정
			const Vector3 inward = glm::normalize(glm::cross(direction, up)); // 광고 패널의 안쪽 방향 벡터, 펜스 세그먼트의 방향과 위쪽 방향의 외적을 통해 계산

			// 펜스 안쪽 면에서 조금 띄워 깊이 충돌을 방지한다.
			Matrix4 adModel = glm::translate(
				Matrix4{ 1.0f }, (from + to) * 0.5f + inward * 0.12f);
			adModel = glm::rotate(
				adModel, glm::atan(-direction.z, direction.x), up);
			// 광고 이미지의 가로:세로 3:1 비율을 유지한다.
			adModel = glm::scale(
				adModel, Vector3{ length, length / 3.0f, 1.0f }); // 광고 패널의 크기를 펜스 세그먼트의 길이에 맞추고, 높이는 가로:세로 3:1 비율을 유지하도록 설정

			shader.setMat4("default", "model", adModel); // 광고 패널의 모델 행렬을 셰이더에 전달
			shader.setVec3("default", "color", Vector3{ 1.0f, 1.0f, 1.0f }); // 광고 패널의 색상을 흰색으로 설정하여 텍스처가 원래 색상으로 보이도록 한다.
			shader.setBool("default", "useTexture", true); // 텍스처 사용을 활성화하여 광고 패널에 텍스처를 적용한다.
			MeshManager::getInstance().drawMesh("fence_ad_panel"); // 광고 패널 메쉬를 그린다.
			shader.setBool("default", "useTexture", false); // 텍스처 사용을 해제하여 다른 객체에 영향을 주지 않도록 한다.
		}
		shader.setVec3("default", "color", Vector3{ 0.05f, 0.25f, 0.15f }); // 펜스 색상으로 복구
	}

	// 뒤에서 그리는 베이스와 선의 색상 복구
	shader.setVec3("default", "color", Vector3{ 1.0f, 1.0f, 1.0f });

	// 1루와 3루 추가
	const Vector3 baseScale{ 0.7f, 0.14f, 0.7f };
	const float cornerOffset = baseScale.x / glm::sqrt(2.0f);
	for (const float side : { 1.0f, -1.0f })
	{
		Matrix4 baseModel{ 1.0f };
		baseModel = glm::translate(baseModel, Vector3{ side * baseEnd.x, 0.075f, baseEnd.z - cornerOffset });
		baseModel = glm::rotate(baseModel, glm::radians(45.0f), Vector3{ 0.0f, 1.0f, 0.0f });
		baseModel = glm::scale(baseModel, baseScale);

		shader.setMat4("default", "model", baseModel);
		MeshManager::getInstance().drawMesh("cube");
	}

	// 2루 추가

	// 2루 베이스
	const Vector3 secondBasePos{ 0.0f, 0.075f, baseEnd.z * 2.0f };
	const Vector3 up{ 0.0f, 1.0f, 0.0f };
	auto& meshes = MeshManager::getInstance();

	Matrix4 secondBaseModel = glm::translate(Matrix4{ 1.0f }, secondBasePos);
	secondBaseModel = glm::rotate(secondBaseModel, glm::radians(45.0f), up);
	secondBaseModel = glm::scale(secondBaseModel, Vector3{ 0.7f, 0.14f, 0.7f });

	shader.setMat4("default", "model", secondBaseModel);
	meshes.drawMesh("cube");

	// 1루와 3루에서 각각 2루로 이어지는 확인용 선
	const Vector3 to{ secondBasePos.x, 0.045f, secondBasePos.z };

	for (const float side : { 1.0f, -1.0f })
	{
		const Vector3 from{ side * baseEnd.x, 0.045f, baseEnd.z - cornerOffset };
		const Vector3 direction = to - from;

		Matrix4 lineModel = glm::translate(Matrix4{ 1.0f }, (from + to) * 0.5f);
		lineModel = glm::rotate(lineModel, glm::atan(direction.x, direction.z), up);
		lineModel = glm::scale(lineModel, Vector3{ 0.16f, 0.08f, glm::length(direction) });

		shader.setMat4("default", "model", lineModel);
		meshes.drawMesh("cube");
	}

}

void Park::update()
{
}
