#pragma once
#include "LCamera.h"

class UICamera : public LCamera
{
public:
	float m_fCameraYaw = 0.0f;
	float m_fCameraPitch = 0.0f;
	float m_fCameraRoll = 0.0f;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};

