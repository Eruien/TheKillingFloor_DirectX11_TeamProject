#pragma once
#include "Zombie.h"

class EnemyTakeDamage : public NPCState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
	void UpdateHPbar();
public:
	bool m_Timer = false;
	bool IsTakeDamageAnimation = false;
	float m_AnimationHP = 51.0f;
	float m_MinusHP = 50.0f;
	float m_DamageRate = 1.0f;
	float m_HeadShootRate = 2.0f;
public:
	EnemyTakeDamage(Zombie* parent);
	virtual ~EnemyTakeDamage();
};

