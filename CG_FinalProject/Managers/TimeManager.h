#pragma once
#include "pch.h"
#include "Singleton.h"

// 고정밀 타이머를 사용해 프레임 간 시간(DeltaTime) 및 FPS를 계산하는 매니저
class TimeManager : public Singleton<TimeManager>
{
	friend class Singleton<TimeManager>;

public:
	// 타이머 초기화 (Core에서 1회 호출)
	void init();

	// 매 프레임 시간 갱신 (Core에서 매 프레임 호출)
	void update();

	// 이전 프레임으로부터 경과된 시간(초 단위) 반환
	// 물리 이동, 속도 연산 시 이 값을 곱해주어야 사양에 관계없이 일정한 속도로 동작함
	// 예: pos += direction * speed * TimeManager::getInstance().getDeltaTime();
	float getDeltaTime() const { return deltaTime; }

	// 현재 초당 프레임 수(FPS) 반환
	int getFPS() const { return FPS; }

private:
	TimeManager();
	~TimeManager() = default;

private:
	LARGE_INTEGER curTime;
	LARGE_INTEGER prevTime;
	LARGE_INTEGER frequancy;

	int callCount;			// 1초간 update 호출 횟수 카운트

	float deltaTime;		// 프레임 간 경과 시간 (Delta Time)
	float accTime;			// 1초 단위 FPS 계산을 위한 누적 시간

	int FPS;				// 계산된 초당 프레임 수
};
