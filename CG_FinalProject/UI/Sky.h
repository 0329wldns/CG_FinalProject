#pragma once
#include "Objects/Object.h"

class Sky : public Object
{
public:
	void update() override;
	void render() const override;
};
