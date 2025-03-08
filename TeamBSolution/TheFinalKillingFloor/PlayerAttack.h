#pragma once
#include "LPlayer.h"

class PlayerAttack : public PlayerState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	PlayerAttack(LPlayer* parent);
	virtual ~PlayerAttack();
};

