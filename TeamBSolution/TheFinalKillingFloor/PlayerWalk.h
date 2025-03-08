#pragma once
#include "LPlayer.h"

class PlayerWalk : public PlayerState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	PlayerWalk(LPlayer* parent);
	virtual ~PlayerWalk();
};
