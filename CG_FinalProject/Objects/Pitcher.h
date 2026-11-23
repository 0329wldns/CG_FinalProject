#pragma once
#include "pch.h"
#include "Objects/Object.h"
#include "Objects/Ball.h"

class Pitcher : public Object
{
public:
	Pitcher();
	~Pitcher() = default;

	void update() override;
	void render() const override;

private:
	// 공 궤적 정점 버퍼
	uint32_t trajectoryVAO{};
	uint32_t trajectoryVBO{};
};