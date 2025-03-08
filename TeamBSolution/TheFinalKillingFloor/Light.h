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
	TVector4			g_vLightDir[g_iNumLight]; // w = light damping(attenuation) : 감쇠
	TVector4			g_vLightPos[g_iNumLight]; // w = light radius
	TVector4			g_vEyeDir[g_iNumLight];// w = light intensity : 강도
	TVector4			g_vEyePos[g_iNumLight];// w = light radius		
};

struct LightInfo
{
	TVector3 m_vPosition;		// 라이트 위치
	TVector3 m_vDirection;		// 라이트 방향
	float m_fRange;				// 광원 유효 거리
	float m_fFalloff;			// 내부 콘과 외부 콘의 휘도 (Luminance)
	float m_fTheta;				// 내부 콘의 각도 범위
	float m_fPhi;				// 외부 콘의 각도 범위
	TVector4 m_DiffuseColor;
};

class Light : public LightInfo
{
public:
	TMatrix m_matInitWorld;
	TMatrix m_matWorld;
	TVector3 m_vInitPosition;		// 초기 라이트 위치
	TVector3 m_vScale;
	TVector3 m_vRotation;
	TVector3 m_vOffset;				// 지형 높이에서 추가된 위치(x, y, z)

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

