#include "BillBoard.h"
#include "KObject.h"
#include "LStd.h"
#include "LGlobal.h"
#include "LInput.h"
BillBoard::BillBoard() : MonoBehaviour(L"BillBoard")
{
}

BillBoard::~BillBoard()
{
}

void BillBoard::Init()
{
}

void BillBoard::Frame()
{
	//if (LInput::GetInstance().GetKey(DIK_1)==3)
	//{
	//	LGlobal::g_pMainCamera->m_vCameraPos.x += LGlobal::g_pMainCamera->m_vCameraPos.x + 0.00001f;
	//}
	//if (LInput::GetInstance().GetKey(DIK_2)==3)
	//{
	//	LGlobal::g_pMainCamera->m_vCameraPos.x -= LGlobal::g_pMainCamera->m_vCameraPos.x + 0.00001f;
	//}
	TMatrix matRotation, matTrans, matScale, worldMat;
		matScale = TMatrix::Identity;
	 D3DXMatrixInverse(&matRotation ,nullptr, &LGlobal::g_pMainCamera->m_matView);
	 //matRotation._41 = 0.0f;
	 //matRotation._42 = 0.0f;
	 //matRotation._43 = 0.0f;
	 //matRotation._44 = 1.0f;

	D3DXMatrixTranslation(&matTrans ,GetGameObject()->m_vPosition.x,
		GetGameObject()->m_vPosition.y,
		GetGameObject()->m_vPosition.z
	);

	 D3DXMatrixScaling(&matScale, GetGameObject()->m_vScale.x,
		 GetGameObject()->m_vScale.y,
		 GetGameObject()->m_vScale.z
	);
	 worldMat = matScale * matRotation * matTrans;
	 GetGameObject()->SetMatrix(&worldMat, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matOrthoProjection);
}
