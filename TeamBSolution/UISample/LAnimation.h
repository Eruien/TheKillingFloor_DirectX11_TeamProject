#pragma once
#include "LManager.h"
#include "LManagerAgr.h"
#include "LDXObject.h"
#include "LMath.h"

struct RectUV
{
	LVector m_Min;
	LVector m_Max;
};

class LAnimation
{
public:
	std::vector<RectUV> m_UVList;
	LTexture* m_AnimationTexture;
public:
	float m_OffsetTime = 0.0f;
	float m_AnimationElapsed = 0.0f;
	float m_AnimationIndex = 0.0f;
public:
	void SetUVAnimation(std::wstring filePath, int spriteCount, float animationTime);
	void SetUVPosition(std::vector<SimpleVertex>& vertexList);
	void SetUVPositionReverse(std::vector<SimpleVertex>& vertexList);
public:
	bool Init();
	bool Frame();
	bool Render(std::vector<SimpleVertex>& vertexList, ID3D11DeviceContext* pImmediateContext, ID3D11Buffer* m_pVertexBuffer);
	bool Release();
public:
	LAnimation();
};