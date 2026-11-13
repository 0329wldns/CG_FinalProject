#pragma once
#include "pch.h"
#include "Componants/Collider.h"

class Collider;

// 모든 게임 월드 오브젝트 및 UI의 최상위 기반 클래스
class Object
{
public:
	Object();
	virtual ~Object();

	// 충돌체가 필요한 오브젝트인 경우 호출하여 생성
	// 생성된 콜라이더는 오브젝트 소멸 시 자동 해제됨
	void createCollider();
	void creaeteAnimator() {};

	// 매 프레임 오브젝트 로직(이동, 애니메이션 등) 업데이트
	// 각 오브젝트마다 고유한 동작을 해야 하므로 순수 가상 함수로 선언
	virtual void update() = 0;

	// 오브젝트 로직 처리 후 콜라이더 위치 등을 최종 동기화
	// 프레임워크 내부에서 호출하며, 자식에서 override하지 않도록 final 선언
	virtual void finalUpdate() final;

	// 마우스 상호작용 이벤트 콜백 (UI나 피킹이 필요한 오브젝트에서 필요시 override)
	virtual void onMouseEnter() {};
	virtual void onMouseLeave() {};
	virtual void onMouseDownLeft() {};
	virtual void onMouseDownRight() {};
	virtual void onMouseUpLeft() {};
	virtual void onMouseUpRight() {};

	// 오브젝트를 그리는 함수
	// 오브젝트마다 고유한 메쉬와 셰이더 세팅이 필요하므로 순수 가상 함수로 선언
	virtual void render() const = 0;

	// 콜라이더 등 부착된 컴포넌트 렌더링 (디버그 와이어프레임 표시 등)
	void componantRender() const;

	// 충돌 이벤트 콜백 함수
	// 충돌 반응 로직이 필요한 오브젝트에서 선택적으로 override
	virtual void onCollision(Collider*) {};		// 충돌 유지 중일 때 매 프레임 호출
	virtual void onCollisionEnter(Collider*) {};	// 처음 충돌이 시작될 때 1회 호출
	virtual void onCollisionExit(Collider*) {};		// 충돌 영역에서 벗어날 때 1회 호출

	// 위치(Pos) 설정 함수 (2D 및 3D 오버로딩)
	void setPos(Vector2 _pos) { pos.x = _pos.x; pos.y = _pos.y; }
	void setPos(int _posX, int _posY) { pos.x = (float)_posX; pos.y = (float)_posY; }
	void setPos(double _posX, double _posY) { pos.x = (float)_posX; pos.y = (float)_posY; }
	void setPos(Vector3 _pos) { pos.x = _pos.x; pos.y = _pos.y; pos.z = _pos.z; }
	void setPos(int _posX, int _posY, int _posZ) { pos.x = (float)_posX; pos.y = (float)_posY; pos.z = (float)_posZ; }
	void setPos(double _posX, double _posY, double _posZ) { pos.x = (float)_posX; pos.y = (float)_posY; pos.z = (float)_posZ; }

	// 크기(Scale) 설정 함수 (2D 및 3D 오버로딩)
	void setScale(Vector2 _scale) { scale.x = _scale.x; scale.y = _scale.y; }
	void setScale(int _scaleX, int _scaleY) { scale.x = (float)_scaleX; scale.y = (float)_scaleY; }
	void setScale(double _scaleX, double _scaleY) { scale.x = (float)_scaleX; scale.y = (float)_scaleY; }
	void setScale(Vector3 _scale) { scale.x = _scale.x; scale.y = _scale.y; scale.z = _scale.z; }
	void setScale(int _scaleX, int _scaleY, int _scaleZ) { scale.x = (float)_scaleX; scale.y = (float)_scaleY; scale.z = (float)_scaleZ; }
	void setScale(double _scaleX, double _scaleY, double _scaleZ) { scale.x = (float)_scaleX; scale.y = (float)_scaleY; scale.z = (float)_scaleZ; }

	// 게터 함수들
	Vector3 getPos() const { return pos; }
	Vector3 getScale() const { return scale; }
	Collider* getCollider() const { return collider; }

	// 오브젝트 생존 여부 확인
	// false인 오브젝트는 현재 프레임 종료 시 씬에서 자동 정리됨
	bool isDead() const { return !alive; }

private:
	// 오브젝트를 삭제 대기 상태로 변경
	// 직접 호출하지 말고 EventFunc의 deleteObject()를 사용할 것
	void setDead() { alive = false; }

private:
	Vector3 pos;
	Vector3 scale;

	Collider* collider;

	bool alive;

	friend class EventManager;
};
