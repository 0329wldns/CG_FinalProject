#include "pch.h"
#include "Scenes/Scene_Start.h"
#include "Managers/KeyManager.h"
#include "EventFunc.h"
#include "Objects/Ball.h"
#include "Objects/StrikeZone.h"
#include "Constants.h"
#include "Managers/ShaderManager.h"
#include "UI/MousePointer.h"

void Scene_Start::processKeyInput()
{
	KeyManager& keyManager{ KeyManager::getInstance() };

	// 직구 던지기
	if (keyManager.getKeyState(KEY::NUM1) == KEY_STATE::TAP)
	{
		Vector3 destination{ STRIKE_ZONE::CENTER };
		ball = new Ball(10.0f, destination, PITCH_TYPE::STRAIGHT);
		createObject(ball, OBJECT_GROUP::BALL);
	}
	// 커브볼 던지기
	if (keyManager.getKeyState(KEY::NUM2) == KEY_STATE::TAP)
	{
		Vector3 destination{ STRIKE_ZONE::CENTER };
		ball = new Ball(120.0f, destination, PITCH_TYPE::CURVE);
		createObject(ball, OBJECT_GROUP::BALL);
	}
	// 종료
	if (keyManager.getKeyState(KEY::ESC) == KEY_STATE::TAP)
	{
		glfwSetWindowShouldClose(Core::getInstance().getWindow(), true);
	}

	// 디버그용 키
	if (keyManager.getKeyState(KEY::F1) == KEY_STATE::TAP)
	{
		showBG = !showBG;
	}

	if (keyManager.getKeyState(KEY::F2) == KEY_STATE::TAP)
	{
		showAxis = !showAxis;
	}

	if (keyManager.getKeyState(KEY::F3) == KEY_STATE::TAP)
	{
		showGridXZ = !showGridXZ;
	}

	if (keyManager.getKeyState(KEY::F4) == KEY_STATE::TAP)
	{
		showGridYZ = !showGridYZ;
	}

	if (keyManager.getKeyState(KEY::F5) == KEY_STATE::TAP)
	{
		showGridXY = !showGridXY;
	}

	if (keyManager.getKeyState(KEY::F6) == KEY_STATE::TAP)
	{
		showTrajectory = !showTrajectory;
	}
	if (keyManager.getKeyState(KEY::F7) == KEY_STATE::TAP)
	{
		camera->toggleTopView();
	}

	if (keyManager.getKeyState(KEY::F11) == KEY_STATE::TAP)
	{
		camera->setPos(Vector3(0.0f, 100.0f, 150.0f));
	}

	if (keyManager.getKeyState(KEY::F12) == KEY_STATE::TAP)
	{
		camera->setPos(Vector3(500.0f, 150.0f, 150.0f));
	}

	// 마우스
	if (keyManager.getKeyState(KEY::MOUSE_L) == KEY_STATE::TAP)
	{
		Vector2 mousePos{ keyManager.getMousePos() };
		MousePointer* mousePointer = new MousePointer(mousePos);
		createUI(mousePointer, UI_GROUP::MOUSE_POINTER);

		Vector3 worldMousePos{ getWorldMousePos() };
		cout << worldMousePos.x << ", " << worldMousePos.y << ", " << worldMousePos.z << endl;
	}
}

Vector3 Scene_Start::getWorldMousePos() const
{
	Vector3 mousePos{ KeyManager::getInstance().getMousePos(), -1.0f };
	mousePos.y = SCREEN_HEIGHT - mousePos.y;
	float aspact{ (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT };

	cout << "Mouse Pos: " << mousePos.x << ", " << mousePos.y << endl;

	Vector3 cameraSpaceMouseDir{};
	cameraSpaceMouseDir.x = (2.0f * mousePos.x / SCREEN_WIDTH - 1.0f) * (glm::tan(glm::radians(30.0f)) * aspact);
	cameraSpaceMouseDir.y = (1.0f - 2.0f * mousePos.y / SCREEN_HEIGHT) * glm::tan(glm::radians(30.0f));
	cameraSpaceMouseDir.z = -1.0f;

	cout << "Camera Space Mouse Dir: " << cameraSpaceMouseDir.x << ", " << cameraSpaceMouseDir.y << ", " << cameraSpaceMouseDir.z << endl;

	Matrix4 viewMatrix{ camera->getViewMatrix() };
	Matrix4 invViewMatrix{ glm::inverse(viewMatrix) };

	Vector3 worldSpaceMouseDir{ invViewMatrix * Vector4(cameraSpaceMouseDir, 0.0f) };

	cout << "World Space Mouse Dir: " << worldSpaceMouseDir.x << ", " << worldSpaceMouseDir.y << ", " << worldSpaceMouseDir.z << endl;

	Vector3 cameraPos{ camera->getPos() };
	Vector3 worldSpaceMousePos{ cameraPos + worldSpaceMouseDir * (-cameraPos.z / worldSpaceMouseDir.z) };
	return worldSpaceMousePos;
}

void Scene_Start::update()
{	
	Scene::update();

}

void Scene_Start::drawBG() const
{
	if (showAxis)
	{
		Scene::drawAxis();
	}
	Scene::drawGrid(showGridXZ, showGridYZ, showGridXY);

}

void Scene_Start::enter()
{
	// 카메라
	camera = new Camera(Vector3(0.0f, 100.0f, 300.0f), Vector3(0.0f, 100.0f, -300.0f));
	createObject(camera, OBJECT_GROUP::CAMERA);

	// 스트라이크 존
	StrikeZone* strikeZone = new StrikeZone();
	createObject(strikeZone, OBJECT_GROUP::STRIKE_ZONE);

	Park* park = new Park();
	createObject(park, OBJECT_GROUP::DEFAULT);
}

void Scene_Start::exit()
{
	reset();
	camera = nullptr;
	ball = nullptr;
}
