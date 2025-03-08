#include "Sample.h"
#include "LGlobal.h"
#include "LSpriteUVObj.h"

void Sample::SpriteUV()
{
	razer = new LSpriteUVObj;
	razer->Set();
	razer->SetScale(TVector3(1000.0f, 1000.0f, 1000.0f));
	razer->SetPos(TVector3(0, 0, 0));
	razer->SetBox(razer->m_vPosition);
	razer->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/effect/razer20.png");
	razer->SetUVAnimation(20, 2.0f);
	razer->Init();
}

bool Sample::Init()
{
	m_DebugCamera = std::make_shared<LDebugCamera>();
	m_DebugCamera->CreateLookAt({ 0.0f, 200.0f, -100.0f }, { 0.0f, 0.0f, 1.0f });
	m_DebugCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);
	LGlobal::g_pMainCamera = m_DebugCamera.get();

	// 디버그 카메라 사용법
	// CreateLookAt 첫번째 인자 조정시 디버그 카메라 위치 조정 가능
	// 조작법
	// WASD QE SPACE
	// 와이어프레임 모드 F1
	SpriteUV();
	return true;
}

bool Sample::Frame()
{
	razer->Frame();
	return true;
}

bool Sample::Render()
{
	razer->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
	razer->Render();
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