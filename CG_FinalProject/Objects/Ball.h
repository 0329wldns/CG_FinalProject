#pragma once
#include "pch.h"
#include "Object.h"

class Ball : public Object
{
	Ball() = default;
	~Ball() = default;

	void update() override;
	void render() const override;

private:
	// 공의 진행방향 벡터
	Vector3 direction{ 0.0f, 0.0f, 1.0f };

	// 공의 속력
	float speed{ 0.0f };
};