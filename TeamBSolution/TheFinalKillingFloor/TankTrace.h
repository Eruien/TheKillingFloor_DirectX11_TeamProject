#pragma once
#include "Tank.h"

class TankTrace : public NPCState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	TankTrace(Tank* parent);
	virtual ~TankTrace();
};

