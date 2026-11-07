#pragma once

// 프로그램 전역에서 단 하나의 인스턴스만 유지되도록 보장하는 싱글톤 템플릿 기반 클래스
// 상속받는 자식 클래스는 반드시 'friend class Singleton<자식클래스명>;'을 선언하고 생성자를 private으로 둘 것
template <typename T>
class Singleton
{
protected:
    Singleton() = default;
    virtual ~Singleton() = default;

public:
    // 복사 및 대입 연산자 금지 (싱글톤 유일성 보장)
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    // 전역 어디서든 유일한 인스턴스를 가져오는 함수
    // 사용 예: TimeManager::getInstance().getDeltaTime();
    static T& getInstance()
    {
        static T instance{};
        return instance;
    }
};
