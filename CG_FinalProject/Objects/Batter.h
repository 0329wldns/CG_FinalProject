#pragma once
#include "Objects/Object.h" // 오브젝트에 대한 기본적인 정보를 담고 있는 헤더 파일

class Batter : public Object
{
public:
	void update() override; // 오브젝트의 상태를 업데이트하는 함수

	void render() const override; // 오브젝트를 화면에 렌더링하는 함수
	// const ovverride는 이 함수가 상속받은 클래스에서 재정의되었음을 나타내며, const는 이 함수가 객체의 상태를 변경하지 않음을 나타냅니다.
};