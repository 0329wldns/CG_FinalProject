#pragma once
#include "pch.h"

static const string PROGRAM_NAME{ "BaseBall Simulation" };

static random_device rd{};
static mt19937 gen(rd());
static uniform_int_distribution<int> intDist(1, 10000);
static uniform_real_distribution<float> realDist(0.0f, 1.0f);

static const int SCREEN_WIDTH = 1920;
static const int SCREEN_HEIGHT = 1080;

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

enum class SCENE_TYPE
{
	START,

	END
};

enum class OBJECT_GROUP
{
	DEFAULT,
	BALL,
	BAT,

	END
};

enum class UI_GROUP
{
	DEFAULT,
	BUTTON,

	END
};