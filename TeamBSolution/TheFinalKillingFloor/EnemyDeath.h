#pragma once
#include "Zombie.h"

class EnemyDeath : public NPCState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	EnemyDeath(Zombie* parent);
	virtual ~EnemyDeath();
};

