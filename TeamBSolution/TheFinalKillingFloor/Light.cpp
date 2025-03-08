#include "Light.h"

void Light::SetValue(TVector3* pvPos, TVector3* pvDir, float* pfRange, TVector4* pvColor, TVector3* pvScale, TVector3* pvRotation, float fTheta, float fPhi)
{
	if (pvPos != NULL)
		m_vInitPosition = *pvPos;
	if (pvDir != NULL)
		m_vDirection = *pvDir;
	if (pfRange != NULL)
		m_fRange = *pfRange;
	if (pvColor != NULL)
		m_DiffuseColor = *pvColor;
	//외부
	m_fTheta = cosf(XMConvertToRadians(fTheta / 2));
	//내부
	m_fPhi = cosf(XMConvertToRadians(fPhi / 2));
	
	if (pvScale != NULL)
		m_vScale = *pvScale;
	if (pvRotation != NULL)
		m_vRotation = *pvRotation;

	TMatrix matScale, matRotation;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	//D3DXMatrixRotationYawPitchRoll(&matRotation, m_vRotation.x, m_vRotation.y, m_vRotation.z);
	D3DXMatrixMultiply(&m_matInitWorld, &matScale, &matRotation);
	
	m_matInitWorld._41 = m_vInitPosition.x;
	m_matInitWorld._42 = m_vInitPosition.y;
	m_matInitWorld._43 = m_vInitPosition.z;
}

bool Light::Frame(float x, float y, float z)
{
	m_vPosition.x = x;
	m_vPosition.y = y;
	m_vPosition.z = z;
	return true;
}

Light::Light(void)
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matInitWorld);

	m_vScale = TVector3(1.0f, 1.0f, 1.0f);
	m_vRotation = TVector3(0.0f, 0.0f, 0.0f);

	m_vInitPosition = TVector3(0.0f, 0.0f, 0.0f);
	m_vPosition = TVector3(0.0f, 0.0f, 0.0f);
	m_vDirection = TVector3(0.0f, -1.0f, 0.0f);

	m_fRange = 200.0f;
	m_fFalloff = 4.0f;

	m_DiffuseColor = TVector4(1.f);

	m_fTheta = (float)(XM_PI / 4.0f);
	m_fPhi = m_fTheta + (float)(XMConvertToRadians(5));

	m_vOffset = TVector3(0.0f, 0.0f, 0.0f);

	m_szPath = L"";
}

bool Light::Frame(TVector3 pos)
{
	m_vPosition.x = pos.x;
	m_vPosition.y = pos.y;
	m_vPosition.z = pos.z;
	return true;
}