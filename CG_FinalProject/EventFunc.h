#pragma once
#include "Object.h"
#include "Constants.h"

void createObject(Object* newObject, OBJECT_GROUP group);
void deleteObject(Object* delObject);
void changeScene(SCENE_TYPE nextScene);