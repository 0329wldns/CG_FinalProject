#pragma once
#include "pch.h"
#include "Objects/Object.h"

class StrikeZone : public Object
{
public:
	StrikeZone();
	StrikeZone(Vector3 _pos);
	~StrikeZone() = default;

	void update() override;
	void render() const override;

private:

};