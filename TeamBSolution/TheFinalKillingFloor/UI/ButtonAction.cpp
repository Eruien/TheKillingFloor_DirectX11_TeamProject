#include "ButtonAction.h"
#include "KObject.h"
#include "TextureList.h"
#include "LWrite.h"
#include "LGlobal.h"
#include "../LScene.h"
#include "UIManager.h"
ButtonAction::ButtonAction(wstring texPaths, wstring bFuntion) : MonoBehaviour(L"ButtonAction")
{
	_texXmlPath = texPaths;
	LoadTextureList(_texXmlPath);
	_function = bFuntion;


	_functionMap[L"MainSceneOption"] = &ButtonAction::MainSceneOption;
	_functionMap[L"MainSceneOptionClose"] = &ButtonAction::MainSceneOptionClose;
	_functionMap[L"RetryButton"] = &ButtonAction::RetryButton;
	_functionMap[L"SoundToggle"] = &ButtonAction::SoundToggle;
	_functionMap[L"ToggleFullScreen"] = &ButtonAction::ToggleFullScreen;


	_functionMap[L"GoSelectScene"] = &ButtonAction::GoSelectScene;
	_functionMap[L"GoInGameScene"] = &ButtonAction::GoInGameScene;
	_functionMap[L"GoMainScene"] = &ButtonAction::GoMainScene;
	_functionMap[L"GoEndScene"] = &ButtonAction::GoEndScene;

}
ButtonAction::~ButtonAction()
{
}
void ButtonAction::Frame()
{
	//-(LINPUT.GetMousePos().y - LGlobal::g_WindowHeight / 2)
	state = GetGameObject()->GetScript<PickingUI>(L"PickingUI")->GetButtonState();


	if (state == PICKING_STATE::NONE)
	{
		GetGameObject()->m_Tex = LManager<LTexture>::GetInstance().Load(_texList->GetTexList()[0]);
		GetGameObject()->m_Tex->Apply();
	}
	else if (state == PICKING_STATE::HOVER)
	{
		GetGameObject()->m_Tex = LManager<LTexture>::GetInstance().Load(_texList->GetTexList()[1]);
		GetGameObject()->m_Tex->Apply();
	}
	else if (state == PICKING_STATE::HOLD)
	{
		GetGameObject()->m_Tex = LManager<LTexture>::GetInstance().Load(_texList->GetTexList()[2]);
		GetGameObject()->m_Tex->Apply();
	}
	else if (state == PICKING_STATE::UP)
	{
		GetGameObject()->m_Tex = LManager<LTexture>::GetInstance().Load(_texList->GetTexList()[0]);
		GetGameObject()->m_Tex->Apply();
		if (!UIManager::GetInstance()._editMode)
		{
			if (_functionMap.find(_function) != _functionMap.end())
			{
				(this->*_functionMap[_function])();
			}
			else {
				std::cout << "Function not found" << std::endl;
			}
		}
	}
	//if (state == PICKING_STATE::NONE)
	//{
	//	GetGameObject()->m_Tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/button1.png");
	//	GetGameObject()->m_Tex->Apply();
	//}
	//else if (state == PICKING_STATE::HOVER)
	//{
	//	GetGameObject()->m_Tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/button2.png");
	//	GetGameObject()->m_Tex->Apply();
	//}
	//else if (state == PICKING_STATE::HOLD)
	//{
	//	GetGameObject()->m_Tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/button3.png");
	//	GetGameObject()->m_Tex->Apply();
	//}
}
void ButtonAction::LoadTextureList(wstring texXmlPath)
{
	_texList = make_shared<TextureList>();
	_texList->Load(texXmlPath);
}

void ButtonAction::MainSceneOption()
{

	vector<shared_ptr<KObject>> group;
	group = UIManager::GetInstance().GetGroup(L"MainOptionMenu");
	for (auto& obj : group)
	{
		obj->SetIsRender(true);
	}
}

void ButtonAction::MainSceneOptionClose()
{
	vector<shared_ptr<KObject>> group;
	group = UIManager::GetInstance().GetGroup(L"MainOptionMenu");
	for (auto& obj : group)
	{
		obj->SetIsRender(false);
	}
}

void ButtonAction::RetryButton()
{
	
	UIManager::GetInstance().Load(L"MainScene.xml");
	
}

void ButtonAction::SoundToggle()
{

	_toggleSound = !_toggleSound;

	if (_toggleSound == false)
	{
		UIManager::GetInstance().GetUIObject(L"T_SoundToggle")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"SOUND OFF");
	}
	if (_toggleSound == true)
	{
		UIManager::GetInstance().GetUIObject(L"T_SoundToggle")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"SOUND ON");
	}
	
	LSoundMgr::GetInstance().GetPtr(L"InGameSound.mp3")->ToggleSound(_toggleSound);
	LSoundMgr::GetInstance().GetPtr(L"BackgroundSound.mp3")->ToggleSound(_toggleSound);
	LSoundMgr::GetInstance().GetPtr(L"GunFire.wav")->ToggleSound(_toggleSound);
	LSoundMgr::GetInstance().GetPtr(L"PlayerStep.wav")->ToggleSound(_toggleSound);
	LSoundMgr::GetInstance().GetPtr(L"SteamPack.wav")->ToggleSound(_toggleSound);
	
}

void ButtonAction::ToggleFullScreen()
{
	_toggleFullScreen = !_toggleFullScreen;
	if (_toggleFullScreen == false)
	{
		UIManager::GetInstance().GetUIObject(L"T_ScreenToggle")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"SCREEN WINDOW");
	}
	if (_toggleFullScreen == true)
	{
		UIManager::GetInstance().GetUIObject(L"T_ScreenToggle")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"SCREEN FULL");
	}
	/*LGlobal::g_pLDevice->m_pSwapChain->ResizeBuffers(LGlobal::g_pLDevice->m_SwapChainDesc.BufferCount,
		1920, 1080, LGlobal::g_pLDevice->m_SwapChainDesc.BufferDesc.Format, LGlobal::g_pLDevice->m_SwapChainDesc.Flags);*/
	LGlobal::g_pSwapChain->SetFullscreenState(_toggleFullScreen, nullptr);
}


void ButtonAction::GoSelectScene()
{
	UIManager::GetInstance().ChangeScene(Event::GOSELECTSCENE);
}

void ButtonAction::GoInGameScene()
{
	UIManager::GetInstance().ChangeScene(Event::GOINGAMESCENE);
}

void ButtonAction::GoMainScene()
{
	UIManager::GetInstance().ChangeScene(Event::GOMAINSCENE);
}

void ButtonAction::GoEndScene()
{
	UIManager::GetInstance().ChangeScene(Event::GOENDSCENE);
}
