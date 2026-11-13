#pragma once
#include "pch.h"
#include "Objects/Object.h"

// 마우스 클릭 위치 시각적 피드백을 위한 UI 오브젝트
class MousePointer : public Object
{
public:
	MousePointer(Vector2 _pos);
	~MousePointer() = default;

	void update() override;
	void render() const override;

private:
	float transparency;
};