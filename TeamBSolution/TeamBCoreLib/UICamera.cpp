#include "UICamera.h"
#include "LInput.h"
#include "LGlobal.h"

bool UICamera::Init()
{
	LCamera::Init();
	return true;
}

bool UICamera::Frame()
{



	DirectX::XMVECTOR gRotation;
	DirectX::XMMATRIX matRotation;
	DirectX::XMMATRIX xmMatView;

	gRotation = DirectX::XMQuaternionRotationRollPitchYaw(m_fCameraPitch, m_fCameraYaw, 0);

	DirectX::XMVECTOR xmCameraPos = DirectX::XMVectorSet(m_vCameraPos.x, m_vCameraPos.y, m_vCameraPos.z, 1.0f);
	matRotation = DirectX::XMMatrixAffineTransformation(DirectX::g_XMOne, DirectX::g_XMZero, gRotation, xmCameraPos);
	xmMatView = DirectX::XMMatrixInverse(NULL, matRotation);
	std::memcpy(&m_matView, &xmMatView, sizeof(DirectX::XMMATRIX));

	UpdateVector();

	return true;
}

bool UICamera::Render()
{
	LCamera::Render();
	return true;
}

bool UICamera::Release()
{
	LCamera::Release();
	return true;
}