#pragma once
#include "Boss.h"

class BossDeath : public NPCState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	BossDeath(Boss* parent);
	virtual ~BossDeath();
};

