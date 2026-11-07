#pragma once
#include "pch.h"
#include "Object.h"

// 야구공 오브젝트 클래스
// 투구 궤적(베지어 곡선) 및 타격 후 물리 이동을 담당
class Ball : public Object
{
public:
	Ball(float _speed, Vector3 _destination);
	~Ball();

	// 매 프레임 공의 이동 및 물리/궤적 연산 처리
	void update() override;

	// 공 3D 메쉬(sphere) 렌더링
	void render() const override;

private:
	// 공의 타격 지점
	Vector3 hitPos;
	
	// 공의 착지 지점
	Vector3 destination;

	// 궤적 연산을 위한 매개변수 t
	float t{ 0.0f };

	// 공 궤적 정점 버퍼
	uint32_t trajectoryVAO{};
	uint32_t trajectoryVBO{};

	// 공의 이동 속력
	// km/h 단위
	float speed{ 0.0f };
};
