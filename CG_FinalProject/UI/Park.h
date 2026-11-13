#pragma once
#include "pch.h"
#include "Objects/Object.h"

class Park : public Object
{

public:
	Park();
	~Park() = default;

	void update() override;
	void render() const override;
};