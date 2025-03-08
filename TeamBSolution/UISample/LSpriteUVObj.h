#pragma once
#include "LPlaneObj.h"
#include "LUtils.h"

struct RectUV
{
	TVector3 m_Min;
	TVector3 m_Max;
};

class LSpriteUVObj : public LPlaneObj
{
public:
	LBox m_Box;
	virtual void SetBox(TVector3& p, float fw, float fh, float fd);
	virtual void SetBox(TVector3& p);
	TVector3 m_Target;
	TVector3 m_fDirection;
	bool m_IsDirectionCal = false;
public:
	std::vector<RectUV> m_UVList;
public:
	float m_OffsetTime = 0.0f;
	float m_AnimationElapsed = 0.0f;
	int m_AnimationIndex = 0;
public:
	void SetTarget(TVector3& t);
	void SetUVAnimation(int spriteCount, float animationTime);
	void SetUVPosition();
	void SetUVPositionReverse();
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	LSpriteUVObj();
};
