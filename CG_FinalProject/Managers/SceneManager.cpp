#include "pch.h"
#include "Managers/SceneManager.h"
#include "Scenes/Scene_Start.h"

SceneManager::SceneManager()
	: scene()
	, curScene(nullptr)
{
}

void SceneManager::init()
{
	scene[(int)SCENE_TYPE::START] = new Scene_Start;

	curScene = scene[(int)SCENE_TYPE::START];
	curScene->enter();
}

void SceneManager::update()
{
	curScene->update();
}

void SceneManager::finalUpdate()
{
	curScene->finalUpdate();
}

void SceneManager::render() const
{
	curScene->render();
}

void SceneManager::changeScene(SCENE_TYPE nextScene)
{
	curScene->exit();
	curScene = scene[(int)nextScene];
	curScene->enter();
}
