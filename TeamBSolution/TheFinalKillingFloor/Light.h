#pragma once
#include "LStd.h"

const int g_iNumLight = 1;
struct LIGHT_CONSTANT_BUFFER1
{
	// Material
	TVector4			g_cAmbientMaterial[g_iNumLight];
	TVector4			g_cDiffuseMaterial[g_iNumLight];
	TVector4			g_cSpecularMaterial[g_iNumLight];
	TVector4			g_cEmissionMaterial[g_iNumLight];
	// Light
	TVector4			g_cAmbientLightColor[g_iNumLight];
	TVector4			g_cDiffuseLightColor[g_iNumLight];
	TVector4			g_cSpecularLightColor[g_iNumLight];
	float               g_bIsZedTime;
	float               padding[3]; // add padding
};
struct LIGHT_CONSTANT_BUFFER2
{
	TMatrix			g_matInvWorld[g_iNumLight];
	TVector4			g_vLightDir[g_iNumLight]; // w = light damping(attenuation) : ����
	TVector4			g_vLightPos[g_iNumLight]; // w = light radius
	TVector4			g_vEyeDir[g_iNumLight];// w = light intensity : ����
	TVector4			g_vEyePos[g_iNumLight];// w = light radius		
};

struct LightInfo
{
	TVector3 m_vPosition;		// ����Ʈ ��ġ
	TVector3 m_vDirection;		// ����Ʈ ����
	float m_fRange;				// ���� ��ȿ �Ÿ�
	float m_fFalloff;			// ���� �ܰ� �ܺ� ���� �ֵ� (Luminance)
	float m_fTheta;				// ���� ���� ���� ����
	float m_fPhi;				// �ܺ� ���� ���� ����
	TVector4 m_DiffuseColor;
};

class Light : public LightInfo
{
public:
	TMatrix m_matInitWorld;
	TMatrix m_matWorld;
	TVector3 m_vInitPosition;		// �ʱ� ����Ʈ ��ġ
	TVector3 m_vScale;
	TVector3 m_vRotation;
	TVector3 m_vOffset;				// ���� ���̿��� �߰��� ��ġ(x, y, z)

public:
	basic_string<TCHAR> m_szName;
	basic_string<TCHAR> m_szPath;

public:
	//HRESULT				Load(ID3D11Device* pDevice, const TCHAR* strFilePath);
	//void				SetPath(TCHAR* pPath);
	void				SetValue(TVector3* pvPos,
						TVector3* pvDir,
						float* pfRange,
						TVector4* pvColor,
						TVector3* pvScale,
						TVector3* pvRotation,
						float fTheta,
						float fPhi);

	bool Frame(float x, float y, float z);
	bool Frame(TVector3 pos);

	Light(void);

};

