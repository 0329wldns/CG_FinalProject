#pragma once
#include "pch.h"
#include "Scene.h"

// 시작 씬 (타이틀 화면 / 모드 및 난이도 선택)
class Scene_Start : public Scene
{
public:
	Scene_Start() = default;
	~Scene_Start() = default;

	// 시작 씬 전용 키 입력 처리 (모드 선택 등)
	void processKeyInput() override;

	// 씬 로직 업데이트 (오브젝트 업데이트를 위해 내부에서 Scene::update() 호출 필수)
	void update() override;

	// 시작 화면 배경 그리기
	void drawBG() const override;

	// 씬 진입 시 초기 오브젝트(UI 버튼, 텍스트 등) 생성
	void enter() override;

	// 씬 탈출 시 리소스 정리
	void exit() override;

private:
};
