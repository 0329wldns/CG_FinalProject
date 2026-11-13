#pragma once
#include "pch.h"
#include "Core.h"
#include "Objects/Object.h"

class Object;

// 오브젝트에 부착되어 충돌 감지 영역을 제공하는 컴포넌트
class Collider {
public:
	Collider();

	// 소유자 오브젝트의 위치를 기준으로 콜라이더의 절대 위치 계산
	// 프레임워크 내부에서 호출되므로 직접 호출할 일 없음
	void finalUpdate();

	// 디버그용 충돌 영역 렌더링 (초록색/빨간색 와이어프레임)
	void render();

	// CollisionManager에서 충돌 판정 시 소유자 오브젝트로 이벤트를 전달하는 함수
	// 충돌이 처음 발생했을 때 호출
	void onCollisionEnter(Collider*);

	// 충돌 유지 중일 때 호출
	void onCollision(Collider*);

	// 충돌에서 벗어날 때 호출
	void onCollisionExit(Collider*);

	// 충돌체 위치 및 크기 설정
	void setPos(Vector2 _pos) { pos = _pos; }
	void setPos(float _posX, float _posY) { pos.x = _posX; pos.y = _posY; }
	void setScale(Vector2 _scale) { scale = _scale; }
	void setScale(float _scaleX, float _scaleY) { scale.x = _scaleX; scale.y = _scaleY; }

	// 소유자 오브젝트 중심점으로부터의 상대적 오프셋 설정
	void setOffset(Vector2 _offset) { offset = _offset; }
	void setOffset(float _offsetX, float _offsetY) { offset.x = _offsetX; offset.y = _offsetY; }

	// 게터 함수들
	Object* getObj() const { return owner; }
	Vector2 getPos() const { return pos; }
	Vector2 getScale() const { return scale; }
	Vector2 getOffset() const { return offset; }
	unsigned int getId() const { return id; }

private:
	Object* owner;		// 이 충돌체를 소유한 부모 오브젝트
	Vector2 pos;		// 실제 월드 좌표 (오브젝트 pos + offset)
	Vector2 scale;		// 충돌 박스 크기
	Vector2 offset;		// 부모 오브젝트 기준 상대 위치

	unsigned int id;	// 콜라이더 고유 식별 번호 (충돌 중복 검사 방지용)

	bool isCol;			// 현재 충돌 중인지 여부 (디버그 색상 변경용)

	friend class Object;
};
