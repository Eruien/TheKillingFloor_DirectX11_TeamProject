#include "Sample.h"
#include "LGlobal.h"
#include "LInput.h"

bool Sample::Init()
{

	UIManager::GetInstance().Load(L"LoadScene.xml");
	UIManager::GetInstance().Load(L"MainScene.xml");
	UIManager::GetInstance().Load(L"SelectScene.xml");
	UIManager::GetInstance().Load(L"IngameScene.xml");
	UIManager::GetInstance().Load(L"EndScene.xml");
	UIManager::GetInstance().Load(L"Shop1.xml");
	UIManager::GetInstance().Load(L"Shop2.xml");



	m_UICamera = std::make_shared<UICamera>();
	m_UICamera->CreateLookAt({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	m_UICamera->m_fCameraPitch = 0.0f;
	m_UICamera->CreateOrthographic((float)LGlobal::g_WindowWidth, (float)LGlobal::g_WindowHeight, -1, 1);
	//LGlobal::g_pUICamera = m_UICamera.get();
	//LScene::GetInstance().SetTransition(Event::GOMAINSCENE);
	LGlobal::g_pSwapChain = m_pSwapChain.Get();
	UIManager::GetInstance().Init(m_pDepthStencilState,m_pDepthStencilStateDisable);


	LScene::GetInstance().FSM(FSMType::SCENE);
	return true;
}

bool Sample::Frame()
{
	
	LScene::GetInstance().Process();

	if (LINPUT.m_KeyStateOld[DIK_F7]== KEY_PUSH)
	{
		UIManager::GetInstance()._editMode = !UIManager::GetInstance()._editMode;

	/*	if (UIManager::GetInstance()._editMode)
		{
			for (auto obj : UIManager::GetInstance().GetUIObjects())
			{
				obj->SetIsRender(true);
			}
		}*/
	}

	if (LINPUT.m_KeyStateOld[DIK_F8] == KEY_PUSH)
	{
		LScene::GetInstance().SetTransition(Event::GOSELECTSCENE);
	}
	return true;
}

bool Sample::Render()
{
	LScene::GetInstance().Render();

	return true;
}

bool Sample::Release()
{
	return true;
}

Sample::~Sample() {}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR IpCmdLine, int nCmdShow)
{
	shared_ptr<Sample> win;
	win = make_shared<Sample>();
	win->_win = win;
	win->SetRegisterWindowClass(hInstance);
	win->SetCreateWindow(L"TeamBProject", LGlobal::g_WindowWidth, LGlobal::g_WindowHeight);
	win->Run();
}


