#include "pch.h"
#include "Managers/EventManager.h"
#include "Managers/SceneManager.h"

EventManager::~EventManager()
{
	for (size_t i = 0; i < vecDead.size(); ++i) delete vecDead[i];
	vecDead.clear();
}

void EventManager::update()
{
	for (size_t i = 0; i < vecDead.size(); ++i) delete vecDead[i];
	vecDead.clear();

	for (size_t i = 0; i < vecEvent.size(); ++i) excute(vecEvent[i]);
	vecEvent.clear();
}

void EventManager::excute(const Event& _event)
{
	switch (_event.event)
	{
	case EVENT_TYPE::CREATE_OBJECT:
	{
		// lParam : object address
		// wParam : group type
		SceneManager::getInstance().getCurScene()->addObject((Object*)_event.lParam, (OBJECT_GROUP)_event.wParam);
		break;
	}
	case EVENT_TYPE::CREATE_UI:
	{
		// lParam : object address
		// wParam : group type
		SceneManager::getInstance().getCurScene()->addUI((Object*)_event.lParam, (UI_GROUP)_event.wParam);
		break;
	}
	case EVENT_TYPE::DELETE_OBJECT:
	{
		// lParam : object address
		Object* deadObject = (Object*)_event.lParam;
		if (!deadObject->isDead())
		{
			vecDead.push_back(deadObject);
			deadObject->setDead();
		}
		break;
	}
	case EVENT_TYPE::SCENE_CHANGE:
	{
		// wParam : next scene type
		SceneManager::getInstance().changeScene((SCENE_TYPE)_event.wParam);
		break;
	}
	}
}
