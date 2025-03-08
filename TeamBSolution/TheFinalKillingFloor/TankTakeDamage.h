#pragma once
#include "Tank.h"

class TankTakeDamage : public NPCState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
	void UpdateHPbar();
public:
	bool m_Timer = false;
	bool IsTakeDamageAnimation = false;
	float m_AnimationHP = 70.0f;
	float m_MinusHP = 30.0f;
	float m_GunDamageRate = 0.1f;
	float m_SwordDamageRate = 0.2f;
	float m_HeadShootRate = 2.0f;
public:
	TankTakeDamage(Tank* parent);
	virtual ~TankTakeDamage();
};

