#pragma once
#include "pch.h"
#include "Object.h"

class StrikeZone : public Object
{
public:
	StrikeZone();
	~StrikeZone() = default;

	void update() override;
	void render() const override;

private:

};