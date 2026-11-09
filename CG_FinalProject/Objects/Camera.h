#pragma once
#include "pch.h"
#include "Object.h"

class Camera : public Object
{
public:
	Camera(Vector3 _pos);
	Camera(Vector3 _pos, Vector3 _at);
	~Camera() = default;

	void update() override;
	void render() const override;

	void setAt(Vector3 _at) { at = _at; }

private:
	Vector3 at;
};