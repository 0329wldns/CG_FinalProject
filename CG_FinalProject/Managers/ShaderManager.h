#pragma once
#include "pch.h"
#include "Singleton.h"

// 셰이더 프로그램(GLSL)의 컴파일, 바인딩 및 Uniform 변수 전달을 총괄하는 매니저
class ShaderManager : public Singleton<ShaderManager>
{
	friend class Singleton<ShaderManager>;

public:
	// 프로젝트에서 사용할 셰이더들을 로드 및 컴파일 (Core에서 1회 호출)
	void init();

	// 특정 셰이더 프로그램을 활성화(glUseProgram)
	// 예: useProgram("default");
	void useProgram(const string& name);

	// 활성화된 셰이더의 mat4 타입 Uniform 변수 설정 (Model, View, Projection 행렬 전달용)
	// 예: setMat4("default", "model", modelMatrix);
	void setMat4(const string& name, const string& uniformName, const glm::mat4& mat);

	// 셰이더의 vec3 타입 Uniform 변수 설정 (색상 등 전달용)
	// 예: setVec3("default", "color", glm::vec3(1.0f, 0.0f, 0.0f));
	void setVec3(const string& name, const string& uniformName, const glm::vec3& vec);

	// 셰이더의 float 타입 Uniform 변수 설정 (투명도 등 전달용)
	void setFloat(const string& name, const string& uniformName, float value);

	// 셰이더의 bool 타입 Uniform 변수 설정
	void setBool(const string& name, const string& uniformName, bool value);

private:
	ShaderManager() = default;
	~ShaderManager() = default;

	// 버텍스 및 프래그먼트 셰이더 파일을 읽어 프로그램을 링크하고 맵에 등록
	void loadShader(const string& name, const string& vertexPath, const string& fragmentPath);

	// 파일 경로로부터 셰이더 소스코드를 읽어오는 유틸리티 함수
	string readShaderFile(const string& filePath);

	// 단일 셰이더(Vertex 또는 Fragment) 컴파일
	uint32_t compileShader(uint32_t type, const string& source);

private:
	unordered_map<string, uint32_t> shaderMap{}; // [셰이더 이름, OpenGL 프로그램 ID] 매핑
};
