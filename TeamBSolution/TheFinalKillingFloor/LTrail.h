#pragma once
#include "LObject.h"

class LTrail : public LObject
{
public:
	int m_iTrailVertexSize = 96;
	int m_iTrailCountSize = 95;
	int m_iVertexCount = 0;
	float m_TimerStart = 1.0;
	float m_TimerEnd = 0.01;
	int m_iCatmullRomIndex[4] = { 0, };
public:
	void RenderTrail(TVector3* localSwordLow, TVector3* localSwordHigh, TMatrix* matSwordParent);
	void InterpolRenderTrail(TVector3* localSwordLow, TVector3* localSwordHigh, TMatrix* matSwordParent);
public:
	bool CreateVertexData() override;
	bool CreateIndexData() override;
};

