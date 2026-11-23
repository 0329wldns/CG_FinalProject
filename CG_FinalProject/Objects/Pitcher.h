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

};