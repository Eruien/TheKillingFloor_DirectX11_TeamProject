#include "Sample.h"
#include "LGlobal.h"

bool Sample::Init()
{
	// Debug Camera 지울시 디버그 카메라 사용불가
	// CreateLookAt() 인자1 카메라 현재위치 변경시 현재 카메라 위치 변경
	// CreatePerspectiveFov() 원근 투영 디버그 카메라 생성
	m_DebugCamera = std::make_shared<LDebugCamera>();
	m_DebugCamera->CreateLookAt({ 0.0f, 200.0f, -100.0f }, { 0.0f, 0.0f, 1.0f });
	m_DebugCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);
	// Core의 부모 카메라에 디버그 카메라의 포인터를 넘겨줌
	// Cored의 부모 카메라에 넘겨주는 포인터에 따라 현재 사용하는 카메라가 달라짐
	LGlobal::g_pMainCamera = m_DebugCamera.get();

	// 매니저를 통해 중복되는걸 로드하지 않고 FBX Mesh에 관한걸 로드한다.
	// FBX는 메인이 되는 Mesh가 하나가 아닌 경우도 있고
	// 텍스처가 여러개가 붙는 경우가 보통이기에 클래스 하나에서 렌더링 하지 않고
	// 모델이 여러개일 경우 서브모델로 나누어서 로드한다.
	// 순서
	// FbxMgr로 fbx파일 안에 있는 mesh를 로드한다.
	// 메인 메쉬가 여러개일 경우 나누어서 메쉬를 로드한다. 
	// 텍스처가 여러개일 경우 하나의 메인메쉬를 나누어서 MeshList에 집어넣는다.
	LFbxObj* fbxObj = LFbxMgr::GetInstance().Load(L"../../res/fbx/zombie/DeadMan_Mesh.fbx");
	ZombieModel = std::make_shared<LModel>();
	// device, context 세팅
	ZombieModel->Set();
	// 로드한 Mesh정보를 model이 렌더링 하기 때문에 넘겨준다.
	ZombieModel->SetLFbxObj(fbxObj);
	// 모델의 자식 세팅
	ZombieModel->SetChild();
	// 애니메이션 스피드 설정 애니메이션 없으면 없어도 되는 부분
	ZombieModel->SetAnimationSpeed(30.0f);

	return true;
}

bool Sample::Frame()
{
	// 좀비 FBX 계산
	// 이 부분은 애니메이션 없으면 없어도 되는 부분
	/*ZombieModel->m_AnimationSpanTime += ZombieModel->GetAnimationSpeed() * LGlobal::g_fSPF;

	if (ZombieModel->m_AnimationSpanTime > ZombieModel->GetLFbxObj()->m_MeshList[0]->m_iEndFrame)
	{
		ZombieModel->m_AnimationSpanTime = 0.0f;
	}

	for (int iMesh = 0; iMesh < ZombieModel->GetLFbxObj()->m_MeshList.size(); iMesh++)
	{
		memcpy(&ZombieModel->m_SubModel[iMesh]->m_matWorld,
			&ZombieModel->GetLFbxObj()->m_MeshList[iMesh]->m_MatrixArray[int(ZombieModel->m_AnimationSpanTime)],
			sizeof(TMatrix));
	}*/
	// 여기까지

	return true;
}

bool Sample::Render()
{
	//좀비 랜더링
	for (auto model : ZombieModel->m_SubModel)
	{
		model->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
		model->Render();
	}

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


