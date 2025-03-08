#pragma once
#include "LPlayer.h"

class PlayerIdle : public PlayerState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	PlayerIdle(LPlayer* parent);
	virtual ~PlayerIdle();
};

