#pragma once
#include "Boss.h"

class BossTakeDamage : public NPCState
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
	float m_GunDamageRate = 0.01f;
	float m_SwordDamageRate = 0.1f;
	float m_HeadShootRate = 2.0f;
public:
	BossTakeDamage(Boss* parent);
	virtual ~BossTakeDamage();
};

