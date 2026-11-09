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

	void init();

	void setAt(Vector3 _at);
	void setAtImediate(Vector3 _at) { at = _at; oldAt = _at; t = 0.0f; }

private:
	Vector3 oldAt;
	Vector3 at;
	float t;
};