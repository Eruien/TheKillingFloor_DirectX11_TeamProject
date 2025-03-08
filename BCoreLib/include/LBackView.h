#pragma once
#include "LCamera.h"
#include "LModel.h"
#include "LInput.h"
#include "LGlobal.h"

class LBackView : public LCamera
{
public:
	LModel* m_TargetModel = nullptr;
	TMatrix m_ModelMatrix;
	const float m_MaxPitch = XM_PI / 7.0f - 0.01f;
public:
	void SetTarget(LModel* model);
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};

