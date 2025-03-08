#pragma once
#include "Zombie.h"

class EnemyPatrol : public NPCState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	EnemyPatrol(Zombie* parent);
	virtual ~EnemyPatrol();
};

