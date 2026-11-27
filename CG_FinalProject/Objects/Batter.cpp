#include "pch.h" // 전처리기 헤더 파일을 포함합니다. 이 파일은 프로젝트의 공통 헤더를 포함하고 있어 컴파일 시간을 단축시킵니다.
#include "Objects/Batter.h" // Batter 클래스의 정의를 포함합니다.
#include "Managers/MeshManager.h" // MeshManager 클래스의 정의를 포함합니다.
#include "Managers/ShaderManager.h" // ShaderManager 클래스의 정의를 포함합니다.

void Batter::update()
{
	// 오브젝트의 상태를 업데이트하는 로직을 여기에 작성합니다.
	// 예: 위치, 속도, 애니메이션 상태 등을 업데이트할 수 있습니다.
}

void Batter::render() const // 오브젝트를 화면에 렌더링하는 함수
{
	// getPos()는 타자의 발 밑 기준점
	const Matrix4 root = glm::translate(Matrix4(1.0f), getPos()); // 오브젝트의 위치를 기준으로 변환 행렬을 생성합니다.
	
	// 발밑에서 몸통 중심까지 올린다.
	const Matrix4 torso = glm::translate(root, Vector3(0.0f, 1.25f, 0.0f)); // 발밑에서 몸통 중심까지 올리는 변환 행렬을 생성합니다.

	// 큐브를 몸통의 너비, 높이, 두께에 맞춘다.
	const Matrix4 torsoModel = glm::scale(torso, Vector3(0.42f, 0.55f, 0.24)); // 몸통의 크기에 맞춘 스케일 변환 행렬을 생성합니다.)

	auto& shader = ShaderManager::getInstance(); // ShaderManager의 인스턴스를 가져옵니다.
	shader.setMat4("default", "model", torsoModel); // 셰이더에 모델 변환 행렬을 설정합니다.
	shader.setVec3("default", "color", Vector3(0.08f, 0.15f, 0.35f)); // 셰이더에 색상을 설정합니다.
	shader.setFloat("default", "transparency", 1.0f); // 셰이더에 투명도를 설정합니다.
	shader.setBool("default", "useTexture", false); // 셰이더에 텍스처 사용 여부를 설정합니다.

	MeshManager::getInstance().drawMesh("cube"); // MeshManager를 사용하여 큐브 메쉬를 렌더링합니다.
}