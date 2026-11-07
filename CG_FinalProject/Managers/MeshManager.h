#pragma once
#include "pch.h"
#include "Singleton.h"

// OpenGL 메쉬 렌더링에 필요한 버퍼 ID 및 정점 수 정보를 담는 구조체
struct MeshData
{
	uint32_t VAO{};
	uint32_t VBO{};
	uint32_t EBO{};
	uint32_t vertexCount{};
};

// 3D OBJ 모델 로드 및 기본 2D/3D 도형(Quad, Line, Circle 등) 그리기를 전담하는 매니저
class MeshManager : public Singleton<MeshManager>
{
	friend class Singleton<MeshManager>;

public:
	// 기본 프리미티브 도형 버퍼 생성 및 리소스(.obj) 등록 (Core에서 1회 호출)
	void init();

	// 3D OBJ 파일을 파싱하여 VBO/VAO를 생성하고 맵에 등록하는 함수
	// name: 식별자 이름 (예: "cube", "sphere", "bat")
	// defaultColor: 정점 색상이 없을 때 적용할 기본 색상
	bool loadOBJ(const std::string& name, const std::string& filePath, const glm::vec3& defaultColor = glm::vec3(1.0f, 1.0f, 1.0f));

	// 등록된 3D 메쉬를 화면에 그리는 함수
	// drawLine이 true이면 와이어프레임(GL_LINES) 모드로 렌더링
	void drawMesh(const std::string& name, bool drawLine = false) const;

	// 등록된 메쉬의 정점 색상을 동적으로 변경 (특정 파트 색상 변경 시 사용)
	void updateMeshColors(const std::string& name, const std::vector<glm::vec3>& colors);
	void resetMeshColors(const std::string& name);

	// 기본 기하 도형 그리기 함수 (UI 또는 디버그용으로 간편하게 사용)
	void drawQuad(bool drawLine = false) const;		// 단위 사각형 그리기 (UI 버튼 등에 활용)
	void drawTriangle(bool drawLine = false) const;	// 단위 삼각형 그리기
	void drawLine() const;							// 단위 선 그리기 (좌표축, 광선 디버깅 등에 활용)
	void drawCircle() const;						// 원 그리기 (SBO 전광판 카운트 표시에 활용)
	void drawDot() const;							// 점 그리기

	// 메쉬 버퍼 정보 가져오기
	MeshData getMeshData(const std::string& name) const
	{
		auto iter{ meshMap.find(name) };
		if (iter == meshMap.end()) return MeshData{};
		return iter->second;
	}

private:
	MeshManager() = default;
	~MeshManager() = default;

private:
	// 공용 기본 프리미티브 버퍼
	uint32_t quadVAO{};
	uint32_t quadVBO{};
	uint32_t quadEBO{};

	uint32_t triangleVAO{};
	uint32_t triangleVBO{};

	uint32_t lineVAO{};
	uint32_t lineVBO{};

	uint32_t circleVAO{};
	uint32_t circleVBO{};

	// 로드된 외부 OBJ 메쉬 보관 테이블 [메쉬 이름, MeshData]
	std::unordered_map<std::string, MeshData> meshMap{};
};
