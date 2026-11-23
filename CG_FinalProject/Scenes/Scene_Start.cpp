#include "pch.h"
#include "Scenes/Scene_Start.h"
#include "Managers/KeyManager.h"
#include "EventFunc.h"
#include "Objects/Ball.h"
#include "Objects/StrikeZone.h"
#include "Constants.h"
#include "Managers/ShaderManager.h"
#include "UI/MousePointer.h"
#include "Objects/Pitcher.h"

void Scene_Start::processKeyInput()
{
	KeyManager& keyManager{ KeyManager::getInstance() };

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

	if (keyManager.getKeyState(KEY::F9) == KEY_STATE::TAP)
	{
		setCamera("top");
	}

	if (keyManager.getKeyState(KEY::F10) == KEY_STATE::TAP)
	{
		setCamera("side");
	}

	if (keyManager.getKeyState(KEY::F11) == KEY_STATE::TAP)
	{
		setCamera("pitcher");
	}

	if (keyManager.getKeyState(KEY::F12) == KEY_STATE::TAP)
	{
		setCamera("batter");
	}
}

Vector3 Scene_Start::getWorldMousePos() const
{
	Camera* camera{ Camera::getCurCamera() };
	Vector3 mousePos{ KeyManager::getInstance().getMousePos(), -1.0f };
	mousePos.y = SCREEN_HEIGHT - mousePos.y;
	float aspact{ (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT };
	float fovy{ camera->getFovy() };

	Vector3 cameraSpaceMouseDir{};
	cameraSpaceMouseDir.x = (2.0f * mousePos.x / SCREEN_WIDTH - 1.0f) * (glm::tan(glm::radians(fovy / 2.0f)) * aspact);
	cameraSpaceMouseDir.y = (1.0f - 2.0f * mousePos.y / SCREEN_HEIGHT) * glm::tan(glm::radians(fovy / 2.0f));
	cameraSpaceMouseDir.z = -1.0f;

	Matrix4 viewMatrix{ camera->getViewMatrix() };
	Matrix4 invViewMatrix{ glm::inverse(viewMatrix) };

	Vector3 worldSpaceMouseDir{ invViewMatrix * Vector4(cameraSpaceMouseDir, 0.0f) };

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
	// 스트라이크 존
	StrikeZone* strikeZone = new StrikeZone();
	createObject(strikeZone, OBJECT_GROUP::STRIKE_ZONE);

	// 경기장
	Park* park = new Park();
	createObject(park, OBJECT_GROUP::DEFAULT);
	
	// 투수
	Pitcher* pitcher = new Pitcher();
	createObject(pitcher, OBJECT_GROUP::DEFAULT);

	// 카메라 초기화
	Camera* topViewCamera = new Camera(Vector3(0.0f, 50.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), true);
	Camera* sideViewCamera = new Camera(Vector3(5.0f, 1.5f, 1.5f), Vector3(0.0f, 1.0f, -3.0f));
	Camera* batterViewCamera = new Camera(Vector3(0.0f, 1.0f, 3.0f), Vector3(0.0f, 1.0f, 0.0f));
	Camera* pitcherViewCamera = new Camera(Vector3(0.0f, 1.7f, -19.0f), Vector3(0.0f, STRIKE_ZONE::CENTER.y, 0.0f), 12.0f);

	addCamera("top", topViewCamera);
	addCamera("side", sideViewCamera);
	addCamera("batter", batterViewCamera);
	addCamera("pitcher", pitcherViewCamera);

	setCamera("pitcher");
}

void Scene_Start::exit()
{
	reset();

	cameras.clear();
}

void Scene_Start::addCamera(const string& name, Camera* camera)
{
	auto iter = cameras.find(name);

	if (cameras.end() == iter)
	{
		cameras.insert(make_pair(name, camera));
		createObject(camera, OBJECT_GROUP::CAMERA);
	}
}

void Scene_Start::setCamera(const string& name)
{
	auto iter = cameras.find(name);
	if (cameras.end() != iter)
	{
		iter->second->useCamera();
	}
}
