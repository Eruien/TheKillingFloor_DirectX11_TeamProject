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
	// text�� ��� �� �����Ӹ��� ���ŵǱ� ������ frame�̳� render���ٰ� ����ؾ� �Ѵ�.
	// �ʿ��� text�� AddText ù��° ���ڿ��� �־��ָ� �ȴ�.
	// �ι�° ������ ��쿣 text�� ��ġ ������ �� �� �ִ�.
	std::wstring text = L"g_fGameTimer�� �ð�";
	text += std::to_wstring(LGlobal::g_fGameTimer);
	
	// �ð� ���
	// SPF 1�����Ӵ� �帣�� �ð� 
	testTimer += LGlobal::g_fSPF;

	text += L"testTimer�� �ð�";
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


