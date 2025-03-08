#include "Sample.h"
#include "LGlobal.h"

bool Sample::Init()
{
	m_DebugCamera = std::make_shared<LDebugCamera>();
	m_DebugCamera->CreateLookAt({ 0.0f, 200.0f, -100.0f }, { 0.0f, 0.0f, 1.0f });
	m_DebugCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);
	LGlobal::g_pMainCamera = m_DebugCamera.get();

	// �Ŵ����� ���� ���� ������ ������ �ߺ��ؼ� �ε���� �ʴ´�.
	moonlight = LSoundMgr::GetInstance().Load(L"../../res/sound/moonlight.mp3");
	// ����
	return true;
}

bool Sample::Frame()
{
	// ���尡 �帣�� ���� �������� �����ش�.
	moonlight->Frame();

	LInput& input = LInput::GetInstance();

	// ���콺 ���� �÷���
	if (input.m_KeyState[VK_LBUTTON] == KEY_PUSH)
	{
		moonlight->Play();
	}

	// ���콺 ������ ����
	if (input.m_KeyState[VK_RBUTTON] == KEY_PUSH)
	{
		moonlight->Stop();
	}
	
	// +Ű ����
	if (input.m_KeyState[VK_OEM_PLUS] == KEY_PUSH)
	{
		moonlight->VolumeUp();
	}

	//-Ű ����
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


