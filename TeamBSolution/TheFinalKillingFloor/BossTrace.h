#pragma once
#include "Boss.h"

class BossTrace : public NPCState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	BossTrace(Boss* parent);
	virtual ~BossTrace();
};

