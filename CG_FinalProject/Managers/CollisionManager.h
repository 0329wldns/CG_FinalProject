#pragma once
#include "pch.h"
#include "Singleton.h"
#include "SceneManager.h"
#include "Object.h"
#include "Collider.h"
#include "Constants.h"

// 두 콜라이더 간의 충돌 쌍(Pair)을 고유한 64비트 정수로 식별하기 위한 공용체
union COLLIDER_ID 
{
	struct 
	{
		unsigned int leftId;
		unsigned int rightId;
	};
	uint64_t id;
};

// 오브젝트 그룹 간의 충돌 감지 및 이벤트(Enter, Stay, Exit) 디스패치를 전담하는 매니저
class CollisionManager : public Singleton<CollisionManager>
{
	friend class Singleton<CollisionManager>;

public:
	// 매 프레임 등록된 충돌 그룹 간의 충돌 검사 실행 (Core에서 매 프레임 호출)
	void update();

	// 특정 두 그룹 간의 충돌 검사 활성화/비활성화 토글
	// 충돌이 필요한 그룹 쌍만 켜두어 불필요한 연산을 줄임
	// 예: CollisionManager::getInstance().checkCollisionGroup(OBJECT_GROUP::BALL, OBJECT_GROUP::BAT);
	void checkCollisionGroup(OBJECT_GROUP left, OBJECT_GROUP right);

	// 두 그룹에 속한 오브젝트들의 콜라이더를 순회하며 충돌 검사 및 이벤트 호출
	void collisionGroupUpdate(OBJECT_GROUP left, OBJECT_GROUP right);

	// 두 콜라이더의 영역이 겹치는지 실제 기하 판정을 수행하는 함수
	bool isCollision(const Collider* leftCol, const Collider* rightCol) const;

	// 충돌 그룹 체크 상태 및 충돌 이력 초기화 (씬 전환 시 호출)
	void reset();

private:
	CollisionManager() = default;
	~CollisionManager() = default;

private:
	map<uint64_t, bool> collisionInfo;				// 이전 프레임 충돌 상태 저장 (Enter/Exit 판별용)
	unsigned int arrCheck[(int)OBJECT_GROUP::END]{};	// 그룹 간 충돌 체크 비트마스크 배열
};
