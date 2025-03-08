#include "Sample.h"
#include "LGlobal.h"

bool Sample::Init()
{


	m_DebugCamera = std::make_shared<LDebugCamera>();
	m_DebugCamera->CreateLookAt({ 0.0f, 700.0f, -500.0f }, { 0.0f, 0.0f, 1.0f });
	m_DebugCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);
	LGlobal::g_pMainCamera = m_DebugCamera.get();

	m_HeightMap = new LHeightMap;
	m_HeightMap->Set();
	m_HeightMap->CreateHeightMap(L"../../res/Heightmap/heightMap513.bmp");

	// 텍스처랑 상관없음 맵 크기를 정하면 그 크기만큼 맵이 만들어진다.
	// 크기는 2의n승 +1 크기만 가능하다.
	// ex 513, 1025,
	LMapDesc MapDesc = {};
	MapDesc.iNumCols = m_HeightMap->m_iNumCols;
	MapDesc.iNumRows = m_HeightMap->m_iNumRows;
	MapDesc.fCellDistance = 1.0f;
	MapDesc.fScaleHeight = 1.0f;
	MapDesc.ShaderFilePath = L"../../res/hlsl/CustomizeMap.hlsl";
	MapDesc.TextureFilePath = L"../../res/map/basecolor.jpg";
	m_HeightMap->Load(MapDesc);

	// 프러스텀 컬리용 트리 
	m_Tree = new LQurdtree;
	m_Tree->Set();
	m_Tree->m_TreeDepth = 2;
	// 프러스텀 컬링할 맵이랑 맵의 크기를 입력
	m_Tree->BuildQurdTree(m_HeightMap, 513, 513);

	return true;
}

bool Sample::Frame()
{
	m_Tree->Frame();

	return true;
}

bool Sample::Render()
{
	m_Tree->Render();

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


