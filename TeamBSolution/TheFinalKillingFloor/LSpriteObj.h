#pragma once
#include "LPlaneObj.h"

struct LSpriteInfo
{
	int iNumRow;
	int iNumColumn;
	float fAnimTimer;
	float fElapsedTimer;
	LVector vPos;
	LVector vScale;
	std::wstring shaderFile;
	std::wstring textureFile;
};

class LSpriteObj : public LPlaneObj
{
public:
	float m_OffsetTime;
	float m_GameTimer;
	int m_AniIndex;
public:
	virtual int GetMaxSize() { return 1; }
public:
	virtual void Load(ID3D11Device* m_pDevice,
	ID3D11DeviceContext* m_pImmediateContext,
		LSpriteInfo info);
};

class LSpriteTexture : public LSpriteObj
{
public:
	std::vector<LTexture*> m_TexList;
public:
	int GetMaxSize() override { return m_TexList.size(); }
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

class LSpriteUV : public LSpriteObj
{
public:
	std::vector<LRectUV> m_UVList;
public:
	int GetMaxSize() override { return m_UVList.size(); }
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

