#pragma once
#include "LCamera.h"

class LDebugCamera : public LCamera
{
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};

