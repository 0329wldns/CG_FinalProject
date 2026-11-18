#pragma once
#include "pch.h"
#include "Objects/Object.h"

class Camera : public Object
{
public:
	Camera(Vector3 _pos);
	Camera(Vector3 _pos, Vector3 _at);
	Camera(Vector3 _pos, Vector3 _at, Vector3 _up);
	Camera(Vector3 _pos, Vector3 _at, Vector3 _up, bool _isOrthographic = false);
	~Camera() = default;

	void update() override;
	void render() const override;

	void init();

	void useCamera() { curCamera = this; }	// 자신을 현재 카메라로 설정

	void setAt(Vector3 _at);
	void setAtImediate(Vector3 _at) { at = _at; oldAt = _at; t = 0.0f; }
	void toggleTopView() { topView = !topView; }// 탑 뷰 모드 추가

	Matrix4 getViewMatrix() const { return viewMatrix; }

private:
	inline static Camera* curCamera{ nullptr };	// 현재 사용 중인 카메라

	Vector3 oldAt;
	Vector3 at;

	Vector3 up{ 0.0f, 1.0f, 0.0f };

	Matrix4 viewMatrix{ 1.0f };

	bool isOrthographic{ false };	// 직교 투영 여부

	float t{ 0.0f };	// 카메라 이동 애니메이션 용 매게변수

	bool topView = false; // 탑 뷰 모드 여부
};