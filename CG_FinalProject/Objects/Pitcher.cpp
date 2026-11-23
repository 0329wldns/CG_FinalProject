#include "pch.h"
#include "Objects/Pitcher.h"
#include "Constants.h"
#include "Managers/KeyManager.h"
#include "Managers/SceneManager.h"
#include "Scenes/Scene_Start.h"
#include "UI/MousePointer.h"
#include "Eventfunc.h"

Pitcher::Pitcher()
{
	setPos(Vector3(0.0f, 0.0f, MOUND_Z));
}

void Pitcher::update()
{
	static PITCH_TYPE pitchType{ PITCH_TYPE::STRAIGHT };

	Scene_Start* curScene{ static_cast<Scene_Start*>(SceneManager::getInstance().getCurScene()) };
	KeyManager& keyManager{ KeyManager::getInstance() };

	// 던질 구종 고르기
	if (keyManager.getKeyState(KEY::NUM1) == KEY_STATE::TAP)
	{
		pitchType = PITCH_TYPE::STRAIGHT;
	}
	if (keyManager.getKeyState(KEY::NUM2) == KEY_STATE::TAP)
	{
		pitchType = PITCH_TYPE::CURVE;
	}

	if (keyManager.getKeyState(KEY::MOUSE_L) == KEY_STATE::TAP)
	{
		Vector2 mousePos{ keyManager.getMousePos() };
		MousePointer* mousePointer = new MousePointer(mousePos);
		createUI(mousePointer, UI_GROUP::MOUSE_POINTER);

		Vector3 worldMousePos{ curScene->getWorldMousePos() };

		Vector3 destination{ worldMousePos };
		Ball* ball = new Ball(140.0f, destination, pitchType);
		createObject(ball, OBJECT_GROUP::BALL);

		cout << worldMousePos.x << ", " << worldMousePos.y << ", " << worldMousePos.z << endl;
	}
}

void Pitcher::render() const
{

}
