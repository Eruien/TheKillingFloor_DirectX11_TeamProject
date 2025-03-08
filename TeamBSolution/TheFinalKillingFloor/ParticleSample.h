#include "LCore.h"
#include "LPlaneObj.h"
#include "LTimer.h"
#include "LCamera.h"
#include "LPlayer.h"
#include "LManager.h"
#include "LManagerAgr.h"
#include "LSpriteObj.h"
#pragma comment(lib, "LCoreLib.lib")

class LParticle
{
public:
	bool m_isLife = true;
	LVector m_vPos;
	LVector m_vScale;
	LVector m_vRotation;
	float m_OffsetTime = 0.0f;
	float m_AniTimer = 0.0f;
	std::wstring m_wsName = L"";
	int m_iIndex = 0;
	int m_MaxFrame = 0;
	int m_AniIndex = 0;
public:
	void Frame()
	{
		if (m_MaxFrame <= 1) return;

		m_AniTimer += g_fGameTimer;
		if (m_AniTimer >= m_OffsetTime)
		{
			m_AniIndex++;
			if (m_AniIndex >= m_MaxFrame)
			{
				m_AniIndex = 0;
				m_isLife = false;
			}
			m_AniTimer = m_AniTimer - m_OffsetTime;
		}
	}
};

class Sample : public LCore
{
public:
	std::map<int, LSpriteObj*> m_SpriteList;
	std::list<LParticle> m_ParticleList;
	LSpriteObj* m_TexObj = nullptr;
	LSpriteObj* m_UVObj = nullptr;
	LObject* m_pNumberObj = nullptr;
	LObject* m_pMapObj = nullptr;
	std::vector<LTexture*> m_TexList;
	std::vector<LObject*> m_NpcList;
	ID3D11BlendState* m_AlphaBlend = nullptr;
	LPlayer* m_pPlayer = nullptr;
	int m_texIndex = 0;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	void CreateBlendState();
public:
	virtual ~Sample();
};

