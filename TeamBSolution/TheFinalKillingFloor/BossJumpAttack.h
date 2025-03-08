#pragma once
#include "Boss.h"

class BossJumpAttack : public NPCState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	BossJumpAttack(Boss* parent);
	virtual ~BossJumpAttack();
};

