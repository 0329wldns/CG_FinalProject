#pragma once
#include "pch.h"
#include "Singleton.h"
#include "Constants.h"

class Object;

// 처리할 지연 이벤트 종류
enum class EVENT_TYPE
{
	CREATE_OBJECT,  // 새 오브젝트 생성 및 씬 등록
	CREATE_UI,		// 새 UI 생성 및 씬 등록
	DELETE_OBJECT,  // 기존 오브젝트 삭제
	SCENE_CHANGE    // 씬 전환
};

// 지연 이벤트 데이터 구조체
struct Event
{
	EVENT_TYPE event{};
	void* lParam{}; // 오브젝트 포인터 등
	int wParam{};   // 그룹 타입, 씬 타입 등 부가 정수 데이터
};

// 프레임 실행 도중 발생하는 생성/삭제/씬전환 요청을 큐에 모아두었다가
// 프레임 끝(모든 업데이트가 완료된 후)에 일괄 처리하여 메모리 충돌과 이터레이터 무효화를 방지하는 매니저
class EventManager : public Singleton<EventManager>
{
	friend class Singleton<EventManager>;

public:
	// 이벤트 큐에 새 이벤트 추가
	// 편의를 위해 직접 호출하기보다 EventFunc.h의 함수들을 사용하는 것을 권장
	void addEvent(const Event& _event) { vecEvent.push_back(_event); }

	// 이번 프레임에 수집된 모든 이벤트를 순차적으로 실행하고 정리
	void update();

	// 단일 이벤트 실행 로직
	void excute(const Event& _event);

private:
	EventManager() = default;
	~EventManager(); // 종료 시 지연 삭제 대기열(vecDead)의 최종 메모리 해제 보장

private:
	vector<Event> vecEvent;     // 현재 프레임에 대기 중인 이벤트 큐
	vector<Object*> vecDead;    // 이번 프레임에 안전하게 삭제된 오브젝트 모음 (메모리 해제용)
};
