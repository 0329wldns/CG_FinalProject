#include "pch.h"
#include "Scene_Start.h"
#include "KeyManager.h"
#include "EventFunc.h"

void Scene_Start::processKeyInput()
{
	if (KeyManager::getInstance().getKeyState(KEY::ESC) == KEY_STATE::TAP)
		glfwSetWindowShouldClose(Core::getInstance().getWindow(), true);
}

void Scene_Start::update()
{	
	processKeyInput();
	Scene::update();
}

void Scene_Start::drawBG() const
{
	Scene::drawAxis();
	Scene::drawGrid();
}

void Scene_Start::enter()
{
	
}

void Scene_Start::exit()
{
	reset();
}