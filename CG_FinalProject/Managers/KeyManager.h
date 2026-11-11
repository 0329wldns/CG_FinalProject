#pragma once
#include "pch.h"
#include "Singleton.h"

// 관리할 키보드 및 마우스 버튼 목록
enum class KEY
{
	NUM1, NUM2, NUM3, NUM4, NUM5,
	NUM6, NUM7, NUM8, NUM9, NUM0,

	W, A, S, D,
	I, J, K, L,

	UP, DOWN, LEFT, RIGHT,

	Q, E, R, T, Y, U, O, P,
	F, G, H,
	Z, X, C, V, B, N, M,

	MINUS, EQUAL,

	SPACE,
	ENTER,
	LEFT_SHIFT,

	F1, F2, F3, F4, 
	F5, F6, F7, F8, 
	F9, F10, F11, F12,

	MOUSE_L, MOUSE_R,
	ESC,
	LAST
};

// 키의 입력 상태 단계
enum class KEY_STATE
{
	NONE,	// 눌려있지 않은 상태
	TAP,	// 이번 프레임에 막 눌린 순간 (1회성 트리거, 타격 스윙/버튼 클릭 등에 적합)
	HOLD,	// 계속 누르고 있는 상태 (지속 이동 등에 적합)
	AWAY	// 누르고 있다가 이번 프레임에 손을 뗀 순간
};

// 각 키별 상태 정보 구조체
struct KeyInfo
{
	KEY_STATE keyState{};
	bool prevPush{};
};

// 키보드 및 마우스 입력 상태를 감지하고 갱신하는 매니저
class KeyManager : public Singleton<KeyManager>
{
	friend class Singleton<KeyManager>;

public:
	// 키 매핑 테이블 초기화 (Core에서 1회 호출)
	void init();

	// 매 프레임 키 및 마우스 입력 상태 동기화 (Core에서 매 프레임 호출)
	void update();

	// 특정 키의 현재 입력 상태를 확인하는 함수
	// 예: if (KeyManager::getInstance().getKeyState(KEY::SPACE) == KEY_STATE::TAP)
	KEY_STATE getKeyState(KEY _key) const { return keyInfo[(int)_key].keyState; }

	// 현재 마우스 커서의 화면 픽셀 좌표 반환 (좌하단 기준)
	Vector2 getMousePos() const { return curMousePos; }

private:
	KeyManager() = default;

private:
	vector<KeyInfo> keyInfo;
	Vector2 curMousePos{};
};
