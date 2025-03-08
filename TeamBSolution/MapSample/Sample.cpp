#include "Sample.h"
#include "LGlobal.h"

bool Sample::Init()
{
	m_DebugCamera = std::make_shared<LDebugCamera>();
	m_DebugCamera->CreateLookAt({ 0.0f, 200.0f, -100.0f }, { 0.0f, 0.0f, 1.0f });
	m_DebugCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);
	LGlobal::g_pMainCamera = m_DebugCamera.get();

	m_CustomMap = new LMap;
	// device, context �Ҵ�
	m_CustomMap->Set();
	// �� ����ü �ʱ�ȭ
	LMapDesc CMapDesc = {};
	// �ؽ�ó�� ������� �� ũ�⸦ ���ϸ� �� ũ�⸸ŭ ���� ���������.
	// ũ��� 2��n�� +1 ũ�⸸ �����ϴ�.
	// ex 513, 1025,
	CMapDesc.iNumCols = 513;
	CMapDesc.iNumRows = 513;
	CMapDesc.fCellDistance = 1.0f;
	CMapDesc.ShaderFilePath = L"../../res/hlsl/CustomizeMap.hlsl";
	CMapDesc.TextureFilePath = L"../../res/map/topdownmap.jpg";
	m_CustomMap->Load(CMapDesc);

	return true;
}

bool Sample::Frame()
{
	m_CustomMap->Frame();

	return true;
}

bool Sample::Render()
{
	m_CustomMap->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
	m_CustomMap->Render();

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


