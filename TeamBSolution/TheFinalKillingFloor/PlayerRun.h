#pragma once
#include "LPlayer.h"

class PlayerRun : public PlayerState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	PlayerRun(LPlayer* parent);
	virtual ~PlayerRun();
};

