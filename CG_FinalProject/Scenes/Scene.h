#pragma once
#include "pch.h"
#include "Object.h"
#include "Constants.h"

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	// 키 입력 처리
	// 씬 로직에 맞게 처리해야 하므로 순수 가상 함수로 선언
	virtual void processKeyInput() = 0;

	// 씬 로직 업데이트
	// 오브젝트 업데이트를 포함하므로 기본 구현 제공
	// 만약 씬 만의 로직이 필요할 경우 override한 함수에서 Scene::update() 호출 후 추가 로직 구현
	virtual void update();

	// 충돌 처리 및 오브젝트 업데이트 후 처리
	void finalUpdate();


	// 여기서 drawBG, drawObject, drawUI 호출
	void render();

	// 씬에 필요한 배경 그리기
	// 씬마다 배경이 다르므로 순수 가상 함수로 선언
	virtual void drawBG() const = 0;

	// 각 씬에 저장된 오브젝트 및 UI를 그리는 함수
	void drawObject() const;
	void drawUI() const;


	// 좌표 축을 그리는 함수, 필요시 호출
	void drawAxis() const;

	// 평면에 그리드 그리는 함수, 필요시 호출
	void drawGrid(bool xz, bool yz, bool xy) const;


	// 씬 진입 및 탈출 시 처리할 로직
	virtual void enter() = 0;	// 씬 진입
	virtual void exit() = 0;	// 씬 탈출

	// 씬에 저장된 오브젝트 및 UI를 초기화하는 함수
	void reset();

	// event를 이용해 호출 될거라 씬에서 직접 호출할 일은 없음
	// EventManager 참고
	void addObject(Object* obj, OBJECT_GROUP group);
	void addUI(Object* obj, UI_GROUP group);

	// 씬에 저장된 오브젝트를 가져오는 함수
	// 변경되지 않도록 const 참조 반환
	const vector<Object*>& getGroupObject(OBJECT_GROUP _group) const { return object[(int)_group]; };

private:
	const void processMouseInput();

private:
	vector<Object*> object[(int)OBJECT_GROUP::END];
	vector<Object*> ui[(int)UI_GROUP::END];

	Object* focusedObject{ nullptr };
};
