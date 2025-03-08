#include "Sample.h"
#include "LGlobal.h"

bool Sample::Init()
{
	// Debug Camera ����� ����� ī�޶� ���Ұ�
	// CreateLookAt() ����1 ī�޶� ������ġ ����� ���� ī�޶� ��ġ ����
	// CreatePerspectiveFov() ���� ���� ����� ī�޶� ����
	m_DebugCamera = std::make_shared<LDebugCamera>();
	m_DebugCamera->CreateLookAt({ 0.0f, 200.0f, -100.0f }, { 0.0f, 0.0f, 1.0f });
	m_DebugCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);
	// Core�� �θ� ī�޶� ����� ī�޶��� �����͸� �Ѱ���
	// Cored�� �θ� ī�޶� �Ѱ��ִ� �����Ϳ� ���� ���� ����ϴ� ī�޶� �޶���
	LGlobal::g_pMainCamera = m_DebugCamera.get();

	// �Ŵ����� ���� �ߺ��Ǵ°� �ε����� �ʰ� FBX Mesh�� ���Ѱ� �ε��Ѵ�.
	// FBX�� ������ �Ǵ� Mesh�� �ϳ��� �ƴ� ��쵵 �ְ�
	// �ؽ�ó�� �������� �ٴ� ��찡 �����̱⿡ Ŭ���� �ϳ����� ������ ���� �ʰ�
	// ���� �������� ��� ����𵨷� ����� �ε��Ѵ�.
	// ����
	// FbxMgr�� fbx���� �ȿ� �ִ� mesh�� �ε��Ѵ�.
	// ���� �޽��� �������� ��� ����� �޽��� �ε��Ѵ�. 
	// �ؽ�ó�� �������� ��� �ϳ��� ���θ޽��� ����� MeshList�� ����ִ´�.
	LFbxObj* fbxObj = LFbxMgr::GetInstance().Load(L"../../res/fbx/zombie/DeadMan_Mesh.fbx");
	ZombieModel = std::make_shared<LModel>();
	// device, context ����
	ZombieModel->Set();
	// �ε��� Mesh������ model�� ������ �ϱ� ������ �Ѱ��ش�.
	ZombieModel->SetLFbxObj(fbxObj);
	// ���� �ڽ� ����
	ZombieModel->SetChild();
	// �ִϸ��̼� ���ǵ� ���� �ִϸ��̼� ������ ��� �Ǵ� �κ�
	ZombieModel->SetAnimationSpeed(30.0f);

	return true;
}

bool Sample::Frame()
{
	// ���� FBX ���
	// �� �κ��� �ִϸ��̼� ������ ��� �Ǵ� �κ�
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
	// �������

	return true;
}

bool Sample::Render()
{
	//���� ������
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


