#include "SceneChange.h"
#include "../LScene.h"
#include "PickingUI.h"
#include "KObject.h"
#include "UIManager.h"
#include "LGlobal.h"

SceneChange::SceneChange(Event event) : MonoBehaviour(L"SceneChange")
{
	_sceneEvent = event;
}

SceneChange::SceneChange() : MonoBehaviour(L"SceneChange")
{
}

SceneChange::~SceneChange()
{
}

void SceneChange::Init()
{

}

void SceneChange::Frame()
{

	PICKING_STATE state = GetGameObject()->GetScript<PickingUI>(L"PickingUI")->GetButtonState();

	if (state == PICKING_STATE::UP)
	{
		if (_sceneEvent == Event::GOMAINSCENE)
		{
			//수정해야함
			//LInput::GetInstance().CursorChange();
			LSoundMgr::GetInstance().GetPtr(L"InGameSound.mp3")->Stop();
			LSoundMgr::GetInstance().GetPtr(L"BackgroundSound.mp3")->Play();
			UIManager::GetInstance().Load(L"MainScene.xml");
			
			LScene::GetInstance().SetTransition(_sceneEvent);
		}
		else if (_sceneEvent == Event::GOINGAMESCENE)
		{
		
			LInput::GetInstance().CursorChange();
			LGlobal::
				->Stop();
			UIManager::GetInstance().Load(L"IngameScene.xml");
			UIManager::GetInstance().AdjustRes();
			LScene::GetInstance().SetTransition(_sceneEvent);
			LScene::GetInstance().m_pActionList[State::INGAMESCENE]->Retry();
			UIManager::GetInstance().GetUIObject(L"C_Ammo")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_BulletCount);
			UIManager::GetInstance().GetUIObject(L"T_Ammo")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(30);
		}
		else if (_sceneEvent == Event::GOENDSCENE)
		{
			LInput::GetInstance().CursorChange();
			UIManager::GetInstance().Load(L"EndScene.xml");
			UIManager::GetInstance().AdjustRes();
			LScene::GetInstance().SetTransition(_sceneEvent);
		}


	}
}


