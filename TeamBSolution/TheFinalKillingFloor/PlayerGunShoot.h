#pragma once
#include "LPlayer.h"

class PlayerGunShoot : public PlayerState
{
public:
	bool Montage(int startFrame, int endFrame);
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	PlayerGunShoot(LPlayer* parent);
	virtual ~PlayerGunShoot();
};

