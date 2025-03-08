#include "LBackView.h"

void LBackView::SetTarget(LModel* model)
{
	m_TargetModel = model;
	m_ModelMatrix = model->m_matControl;
}

bool LBackView::Init()
{
	LCamera::Init();
	return true;
}

bool LBackView::Frame()
{
	if (m_TargetModel == nullptr)
		return false;
	//m_ModelMatrix = m_TargetModel->m_matControl;
	float x = LInput::GetInstance().m_vOffset.x;
	float y = LInput::GetInstance().m_vOffset.y;
	m_fCameraYaw += DirectX::XMConvertToRadians(x * 0.1f);
	m_fCameraPitch += DirectX::XMConvertToRadians(y * 0.1f);

	DirectX::XMVECTOR gRotation;
	DirectX::XMMATRIX matRotation;
	DirectX::XMMATRIX xmMatView;
	DirectX::XMMATRIX xmInverse;
	TMatrix matTemp;
	TVector3 backView;
	//gRotation = DirectX::XMQuaternionRotationRollPitchYaw(m_fCameraPitch, m_fCameraYaw, 0);

	//DirectX::XMVECTOR xmCameraPos = DirectX::XMVectorSet(m_vCameraPos.x, m_vCameraPos.y, m_vCameraPos.z, 1.0f);
	//matRotation = DirectX::XMMatrixAffineTransformation(DirectX::g_XMOne, DirectX::g_XMZero, gRotation, xmCameraPos);
	//xmMatView = DirectX::XMMatrixInverse(NULL, matRotation);
	//std::memcpy(&m_matView, &xmMatView, sizeof(DirectX::XMMATRIX));

	gRotation = DirectX::XMQuaternionRotationRollPitchYaw(m_fCameraPitch, m_fCameraYaw, 0);
	DirectX::XMVECTOR xmModelPos = DirectX::XMVectorSet(
		m_TargetModel->m_matControl._41,
		m_TargetModel->m_matControl._42,
		m_TargetModel->m_matControl._43,
		1.0f);	
	matRotation = DirectX::XMMatrixAffineTransformation(DirectX::g_XMOne, DirectX::g_XMZero, gRotation, xmModelPos);
	
	matTemp = m_ModelMatrix * matRotation;
	//std::memcpy(&matTemp, &xmInverse, sizeof(DirectX::XMMATRIX));
	m_TargetModel->m_matControl = matTemp;

	if (LINPUT.m_MouseState[1] > KEY_PUSH
		&& m_TargetModel->m_Type == PlayerType::GUN)
	{
		// 카메라 추가 회전 45도
		gRotation = DirectX::XMQuaternionRotationRollPitchYaw(0, DirectX::XMConvertToRadians(315), 0);
		DirectX::XMVECTOR xmCameraPos = DirectX::XMVectorSet(m_TargetModel->m_matControl._41,
			m_TargetModel->m_matControl._42,
			m_TargetModel->m_matControl._43,
			1.0f);
		matRotation = DirectX::XMMatrixAffineTransformation(DirectX::g_XMOne, DirectX::g_XMZero, gRotation, xmModelPos);
		matTemp = m_ModelMatrix * matRotation;
		m_vCameraPos.x = matTemp._41;
		m_vCameraPos.y = matTemp._42;
		m_vCameraPos.z = matTemp._43;



		m_fCameraPitch = max(-m_MaxPitch, min(m_MaxPitch, m_fCameraPitch));
		/*m_vCameraPos.x = m_TargetModel->m_matControl._41;
		m_vCameraPos.y = m_TargetModel->m_matControl._42;
		m_vCameraPos.z = m_TargetModel->m_matControl._43;*/
		// 카메라 벽 매몰 방지


		backView = m_TargetModel->m_matControl.Forward() * 75.f;
		m_vCameraPos -= backView;
		m_vCameraPos.y += 38.f;
		m_vCameraPos.x -= m_TargetModel->m_matControl.Right().x * 33.3;
		m_vCameraPos.z -= m_TargetModel->m_matControl.Right().z * 33.3;
	}
	else
	{
		m_fCameraPitch = max(-m_MaxPitch, min(m_MaxPitch, m_fCameraPitch));

		m_vCameraPos.x = matTemp._41;
		m_vCameraPos.y = matTemp._42;
		m_vCameraPos.z = matTemp._43;

		backView = m_TargetModel->m_matControl.Forward() * 200.f;
		m_vCameraPos -= backView;
		m_vCameraPos.y += 38.f;
	}
	
	if (m_vCameraPos.x < -990.0f)
	{
		m_vCameraPos.x = -990.0f;
		m_vCameraPos.y += 1.0f;
	}
	if (m_vCameraPos.x > 990.0f)
	{
		m_vCameraPos.x = 990.0f;
		m_vCameraPos.y += 1.0f;
	}
	if (m_vCameraPos.z < -990.0f)
	{
		m_vCameraPos.z = -990.0f;
		m_vCameraPos.y += 1.0f;
	}
	if (m_vCameraPos.z > 990.0f)
	{
		m_vCameraPos.z = 990.0f;
		m_vCameraPos.y += 1.0f;
	}
	CreateLookAt(m_vCameraPos, m_vCameraPos + backView);

	UpdateVector();

	return true;
}

bool LBackView::Render()
{
	LCamera::Render();
	return true;
}

bool LBackView::Release()
{
	LCamera::Release();
	return true;
}