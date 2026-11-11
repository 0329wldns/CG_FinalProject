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
		ball = new Ball(140.0f, destination);
		createObject(ball, OBJECT_GROUP::BALL);
		
		Vector3 midPos{ (Vector3(0.0f, 0.0f, 0.0f) + destination) * 0.5f};	// 중간 위치, 2번째 컨트롤 포인트
		midPos.y += 1000.0f;	// 일단 대충 중간 점에서 500 위로 올림

		// 공을 치면 카메라가 공의 최대 높이 지점을 쳐다봄
		camera->setAt(midPos);
	}

	if (KeyManager::getInstance().getKeyState(KEY::ESC) == KEY_STATE::TAP)
	{
		glfwSetWindowShouldClose(Core::getInstance().getWindow(), true);
	}
}

void Scene_Start::update()
{	
	Scene::update();
	if (ball)
	{
		// 공이 70%이상 날아가면 카메라가 착지 지점을 쳐다봄
		if (ball->getT() > 0.7f)
		{
			camera->setAt(ball->getDestination());
		}
	}
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
