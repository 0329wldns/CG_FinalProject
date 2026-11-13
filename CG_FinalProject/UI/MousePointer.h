#pragma once
#include "pch.h"
#include "Objects/Object.h"

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