#include "Sample.h"
#include "LGlobal.h"

bool Sample::Init()
{
	m_DebugCamera = std::make_shared<LDebugCamera>();
	m_DebugCamera->CreateLookAt({ 0.0f, 200.0f, -100.0f }, { 0.0f, 0.0f, 1.0f });
	m_DebugCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);
	LGlobal::g_pMainCamera = m_DebugCamera.get();

	// 매니저로 관리 같은 파일이 있으면 중복해서 로드되지 않는다.
	moonlight = LSoundMgr::GetInstance().Load(L"../../res/sound/moonlight.mp3");
	// 실행
	return true;
}

bool Sample::Frame()
{
	// 사운드가 흐르는 도중 안정감을 더해준다.
	moonlight->Frame();

	LInput& input = LInput::GetInstance();

	// 마우스 왼쪽 플레이
	if (input.m_KeyState[VK_LBUTTON] == KEY_PUSH)
	{
		moonlight->Play();
	}

	// 마우스 오른쪽 종료
	if (input.m_KeyState[VK_RBUTTON] == KEY_PUSH)
	{
		moonlight->Stop();
	}
	
	// +키 증가
	if (input.m_KeyState[VK_OEM_PLUS] == KEY_PUSH)
	{
		moonlight->VolumeUp();
	}

	//-키 감소
	if (input.m_KeyState[VK_OEM_MINUS] == KEY_PUSH)
	{
		moonlight->VolumeDown();
	}

	return true;
}

bool Sample::Render()
{
	return true;
}

bool Sample::Release()
{
	return true;
}

Sample::~Sample() {}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR IpCmdLine, int nCmdShow)
{
	Sample win;
	win.SetRegisterWindowClass(hInstance);
	win.SetCreateWindow(L"TeamBProject", 800, 600);
	win.Run();
}


