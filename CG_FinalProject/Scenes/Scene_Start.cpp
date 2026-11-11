#include "pch.h"
#include "Scene_Start.h"
#include "KeyManager.h"
#include "EventFunc.h"
#include "Ball.h"

void Scene_Start::processKeyInput()
{
	if (KeyManager::getInstance().getKeyState(KEY::SPACE) == KEY_STATE::TAP)
	{
		Vector3 destination{ 0.0f, 0.0f, -1800.0f };
		ball = new Ball(140.0f, destination, PITCH_TYPE::CURVE);
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
	park.render();
	//Scene::drawAxis();
	Scene::drawGrid();

}

void Scene_Start::enter()
{
	camera = new Camera(Vector3(0.0f, 100.0f, 500.0f), Vector3(0.0f, 0.0f, -300.0f));
	createObject(camera, OBJECT_GROUP::CAMERA);
}

void Scene_Start::exit()
{
	reset();
	delete camera;
	camera = nullptr;
	ball = nullptr;
}
