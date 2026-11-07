#pragma once
#include "pch.h"
#include "Singleton.h"

// GLFW 윈도우 생성, OpenGL 컨텍스트 초기화 및 메인 게임 루프를 총괄하는 엔진 코어 클래스
class Core : public Singleton<Core>
{
    friend class Singleton<Core>;

public:
    // GLFW/GLEW 초기화, 윈도우 생성, 각 매니저 init() 일괄 호출 (성공 시 0, 실패 시 -1 반환)
    int init();

    // 메인 루프 1프레임을 실행 (update -> finalUpdate -> render 순서로 진행)
    void progress();

    // 프로그램 실행 유지 여부 확인 (창이 닫히거나 종료 조건 충족 시 false 반환)
    bool isRunning() const;

    // GLFW 윈도우 핸들 반환
    GLFWwindow* getWindow() const { return window; }

private:
    Core() = default;
    ~Core();

    // 매 프레임 시간, 입력, 현재 씬 로직 업데이트
    void update();

    // 매 프레임 충돌 검사 및 이벤트 큐 처리
    void finalUpdate();

    // 현재 씬 렌더링 및 더블 버퍼링(glfwSwapBuffers) 수행
    void render();

private:
    GLFWwindow* window{}; // 생성된 GLFW 윈도우 포인터
};
