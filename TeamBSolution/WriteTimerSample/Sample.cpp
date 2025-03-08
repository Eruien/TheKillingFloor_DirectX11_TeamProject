#include "Sample.h"
#include "LGlobal.h"

bool Sample::Init()
{
	m_DebugCamera = std::make_shared<LDebugCamera>();
	m_DebugCamera->CreateLookAt({ 0.0f, 200.0f, -100.0f }, { 0.0f, 0.0f, 1.0f });
	m_DebugCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);
	LGlobal::g_pMainCamera = m_DebugCamera.get();


	return true;
}

bool Sample::Frame()
{
	// text의 경우 매 프레임마다 갱신되기 때문에 frame이나 render에다가 사용해야 한다.
	// 필요한 text를 AddText 첫번째 인자에다 넣어주면 된다.
	// 두번째 인자의 경우엔 text의 위치 조정을 할 수 있다.
	std::wstring text = L"g_fGameTimer의 시간";
	text += std::to_wstring(LGlobal::g_fGameTimer);
	
	// 시간 계산
	// SPF 1프레임당 흐르는 시간 
	testTimer += LGlobal::g_fSPF;

	text += L"testTimer의 시간";
	text += std::to_wstring(testTimer);

	LWrite::GetInstance().AddText(text, 0.0f, 300.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	
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


