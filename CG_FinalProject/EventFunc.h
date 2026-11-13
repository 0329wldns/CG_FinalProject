#pragma once
#include "Objects/Object.h"
#include "Constants.h"

// 프레임워크 전역에서 안전하게 오브젝트 및 씬을 제어하기 위한 편의 함수 모음
// 게임 로직 실행 중 즉시 삭제/생성 시 발생하는 메모리 충돌을 막기 위해 EventManager를 통해 지연 처리됨

// 새 오브젝트, UI를 생성하여 지정된 그룹에 추가하는 함수
void createObject(Object* newObject, OBJECT_GROUP group);
void createUI(Object* newObject, UI_GROUP group);

// 오브젝트를 안전하게 삭제 대기열에 등록하는 함수 (현재 프레임 종료 시 메모리 해제)
void deleteObject(Object* delObject);

// 다음 씬으로 전환을 요청하는 함수
void changeScene(SCENE_TYPE nextScene);
