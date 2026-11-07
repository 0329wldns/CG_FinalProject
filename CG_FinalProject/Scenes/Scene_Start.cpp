#include "pch.h"
#include "Scene_Start.h"
#include "KeyManager.h"
#include "EventFunc.h"
#include "Ball.h"

void Scene_Start::processKeyInput()
{
	if (KeyManager::getInstance().getKeyState(KEY::SPACE) == KEY_STATE::TAP)
	{
		Vector3  destination{ realDist(gen) * 500.0f, 0.0f, -realDist(gen) * 5000.0f };
		Ball* ball{ new Ball(140.0f, destination) };
		createObject(ball, OBJECT_GROUP::BALL);
	}

	if (KeyManager::getInstance().getKeyState(KEY::ESC) == KEY_STATE::TAP)
	{
		glfwSetWindowShouldClose(Core::getInstance().getWindow(), true);
	}
}

void Scene_Start::update()
{	
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