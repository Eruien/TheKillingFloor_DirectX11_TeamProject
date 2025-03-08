#pragma once
#include "LCamera.h"
#include "LModel.h"

class LModelCamera : public LCamera
{
public:
	LModel* m_TargetModel = nullptr;
	TMatrix m_ModelMatrix;
	float m_fRadius = 300.0f;
	const float m_MaxPitch = XM_PI / 2.0f - 0.01f;
public:
	void SetTarget(LModel* model);
	void SetTargetPos(TVector3 vPos);
public:
	void SetViewMatrix(TVector3 vPos, TVector3 vTarget, TVector3 vUp);
	void Update(TVector4 vDirValue = TVector4(0, 0, 0, 0));
public:
	bool Frame() override;
public:
	LModelCamera();
	virtual ~LModelCamera();
};

