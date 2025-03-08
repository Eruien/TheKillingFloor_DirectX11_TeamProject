#pragma once
#include "Tank.h"

class TankDeath : public NPCState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	TankDeath(Tank* parent);
	virtual ~TankDeath();
};

