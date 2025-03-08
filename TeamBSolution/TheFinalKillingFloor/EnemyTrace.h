#pragma once
#include "Zombie.h"

class EnemyTrace : public NPCState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	EnemyTrace(Zombie* parent);
	virtual ~EnemyTrace();
};

