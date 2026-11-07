#pragma once
#include "pch.h"
#include "Singleton.h"
#include "Scene.h"
#include "Constants.h"

// 씬(Scene)의 등록, 전환 및 현재 씬의 생명주기를 총괄하는 매니저
class SceneManager : public Singleton<SceneManager>
{
	friend class Singleton<SceneManager>;

public:
	// 프로젝트에 존재하는 모든 씬들을 생성하여 배열에 등록 (Core에서 1회 호출)
	void init();

	// 현재 활성화된 씬의 로직 업데이트
	void update();

	// 충돌 및 오브젝트 최종 위치 동기화
	void finalUpdate();

	// 현재 씬 렌더링
	void render() const;

	// 씬 전환 함수
	// 이전 씬의 exit()를 호출하고 새 씬의 enter()를 호출하여 안전하게 전환
	// 직접 호출하기보다 EventFunc의 changeScene()을 통해 이벤트로 호출하는 것을 권장
	void changeScene(SCENE_TYPE nextScene);

	// 현재 활성화된 씬 객체 포인터 반환
	Scene* getCurScene() const { return curScene; }

private:
	SceneManager();
	~SceneManager() = default;

private:
	Scene* scene[(int)SCENE_TYPE::END];	// 등록된 모든 씬의 포인터 배열
	Scene* curScene;					// 현재 실행 중인 씬
};
