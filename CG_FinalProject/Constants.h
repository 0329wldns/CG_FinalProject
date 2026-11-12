#pragma once
#include "pch.h"

// 프로그램 및 윈도우 타이틀 이름
static const char PROGRAM_NAME[] { "BaseBall Simulation" };

// 난수 생성기 유틸리티
static random_device rd{};
static mt19937 gen(rd());
static uniform_int_distribution<int> intDist(1, 10000);
static uniform_real_distribution<float> realDist(0.0f, 1.0f);

// 윈도우 해상도 및 카메라 최대 가시 범위
static const int SCREEN_WIDTH = 1920;
static const int SCREEN_HEIGHT = 1080;
static const float visibleRange = 10000.0f;

// 자주 사용하는 기본 색상 벡터 정의 (RGB 0.0f ~ 1.0f)
static const Color COLOR_RED{ 1.0f, 0.0f, 0.0f };
static const Color COLOR_ORANGE{ 1.0f, 0.5f, 0.0f };
static const Color COLOR_YELLOW{ 1.0f, 1.0f, 0.0f };
static const Color COLOR_GREEN{ 0.0f, 1.0f, 0.0f };
static const Color COLOR_BLUE{ 0.0f, 0.0f, 1.0f };
static const Color COLOR_PURPLE{ 0.5f, 0.0f, 1.0f };
static const Color COLOR_CYAN{ 0.0f, 1.0f, 1.0f };
static const Color COLOR_MAGENTA{ 1.0f, 0.0f, 1.0f };
static const Color COLOR_BLACK{ 0.0f, 0.0f, 0.0f };
static const Color COLOR_GRAY{ 0.5f, 0.5f, 0.5f };
static const Color COLOR_WHITE{ 1.0f, 1.0f, 1.0f };

// 등록된 씬 식별자 (새로운 씬을 만들 때 END 앞에 항목을 추가)
enum class SCENE_TYPE
{
	START,		// 시작/메뉴 씬

	END			// 씬 개수 파악용
};

// 월드 오브젝트 충돌 및 렌더링 레이어 그룹 (END 앞에 추가)
enum class OBJECT_GROUP
{
	DEFAULT,	// 일반 배경 및 기타 오브젝트
	CAMERA,		// 카메라 관련 오브젝트
	BALL,		// 야구공
	BAT,		// 야구 배트
	STRIKE_ZONE,	// 스트라이크 존

	END			// 그룹 개수 파악용
};

// 2D UI 렌더링 레이어 그룹 (END 앞에 추가)
// UI는 깊이 테스트 빼고, 오브젝트 보다 나중에 랜더링 함 -> 맨 위에 그려짐
enum class UI_GROUP
{
	DEFAULT,	// 일반 2D HUD 패널, 텍스트
	BUTTON,		// 클릭 가능한 버튼 UI

	END			// 그룹 개수 파악용
};

namespace STRIKE_ZONE
{
	static const float WIDTH{ 43.0f };
	static const float HEIGHT{ 51.7f };
	static const float DEPTH{ 0.1f };
	static const Vector3 CENTER{ 0.0f, 74.5f, 0.0f };
}