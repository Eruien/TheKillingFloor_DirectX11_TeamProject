#include "LDebugCamera.h"
#include "LInput.h"
#include "LGlobal.h"

bool LDebugCamera::Init()
{
	LCamera::Init();
	return true;
}

bool LDebugCamera::Frame()
{
	
	if(g_InputData.bSpace)
		m_fSpeed += LGlobal::g_fSPF * m_fSpeed;

	m_fSpeed -= LGlobal::g_fSPF * m_fSpeed * 0.5f;
	m_fSpeed = max(10.0f, m_fSpeed);

	if (g_InputData.bWKey)
	{
		TVector3 vOffset = m_vLook * LGlobal::g_fSPF * m_fSpeed;
		m_vCameraPos = m_vCameraPos + vOffset;
	}

	if (g_InputData.bSKey)
	{
		TVector3 vOffset = m_vLook * LGlobal::g_fSPF * m_fSpeed;
		m_vCameraPos = m_vCameraPos - vOffset;
	}

	if (g_InputData.bAKey)
	{
		TVector3 vOffset = m_vRight * LGlobal::g_fSPF * m_fSpeed;
		m_vCameraPos = m_vCameraPos - vOffset;
	}

	if (g_InputData.bDKey)
	{
		TVector3 vOffset = m_vRight * LGlobal::g_fSPF * m_fSpeed;
		m_vCameraPos = m_vCameraPos + vOffset;
	}

	if (g_InputData.bQKey)
	{
		TVector3 vOffset = m_vUp * LGlobal::g_fSPF * m_fSpeed;
		m_vCameraPos = m_vCameraPos + vOffset;
	}

	if (g_InputData.bEKey)
	{
		TVector3 vOffset = m_vUp * LGlobal::g_fSPF * m_fSpeed;
		m_vCameraPos = m_vCameraPos - vOffset;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_W] == KEY_PUSH)
	{
		return false;
	}
		

	if (LInput::GetInstance().m_MouseState[0] == KEY_HOLD)
	{
		float x = LInput::GetInstance().m_vOffset.x;
		float y = LInput::GetInstance().m_vOffset.y;
		m_fCameraYaw += DirectX::XMConvertToRadians(x * 0.1f);
		m_fCameraPitch += DirectX::XMConvertToRadians(y * 0.1f);
	}
	
	
	/*if (LInput::GetInstance().m_dwKeyState[VK_SPACE] == DWORD(KeyState::KEY_HOLD))
	{
		m_fSpeed += LGlobal::g_fSPF * m_fSpeed;
	}

	m_fSpeed -= LGlobal::g_fSPF * m_fSpeed * 0.5f;
	m_fSpeed = max(10.0f, m_fSpeed);

	if (LInput::GetInstance().m_dwKeyState['W'] == DWORD(KeyState::KEY_HOLD))
	{
		TVector3 vOffset = m_vLook * LGlobal::g_fSPF * m_fSpeed;
		m_vCameraPos = m_vCameraPos + vOffset;
	}

	if (LInput::GetInstance().m_dwKeyState['S'] == DWORD(KeyState::KEY_HOLD))
	{
		TVector3 vOffset = m_vLook * LGlobal::g_fSPF * m_fSpeed;
		m_vCameraPos = m_vCameraPos - vOffset;
	}

	if (LInput::GetInstance().m_dwKeyState['A'] == DWORD(KeyState::KEY_HOLD))
	{
		TVector3 vOffset = m_vRight * LGlobal::g_fSPF * m_fSpeed;
		m_vCameraPos = m_vCameraPos - vOffset;
	}

	if (LInput::GetInstance().m_dwKeyState['D'] == DWORD(KeyState::KEY_HOLD))
	{
		TVector3 vOffset = m_vRight * LGlobal::g_fSPF * m_fSpeed;
		m_vCameraPos = m_vCameraPos + vOffset;
	}

	if (LInput::GetInstance().m_dwKeyState['Q'] == DWORD(KeyState::KEY_HOLD))
	{
		TVector3 vOffset = m_vUp * LGlobal::g_fSPF * m_fSpeed;
		m_vCameraPos = m_vCameraPos + vOffset;
	}

	if (LInput::GetInstance().m_dwKeyState['E'] == DWORD(KeyState::KEY_HOLD))
	{
		TVector3 vOffset = m_vUp * LGlobal::g_fSPF * m_fSpeed;
		m_vCameraPos = m_vCameraPos - vOffset;
	}

	if (LInput::GetInstance().m_dwKeyState[VK_LBUTTON] == DWORD(KeyState::KEY_HOLD))
	{
		float x = LInput::GetInstance().m_vOffset.x;
		float y = LInput::GetInstance().m_vOffset.y;
		m_fCameraYaw += DirectX::XMConvertToRadians(x * 0.1f);
		m_fCameraPitch += DirectX::XMConvertToRadians(y * 0.1f);
	}*/

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

bool LDebugCamera::Render()
{
	LCamera::Render();
	return true;
}

bool LDebugCamera::Release()
{
	LCamera::Release();
	return true;
}