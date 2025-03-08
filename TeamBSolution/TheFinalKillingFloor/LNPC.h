#pragma once
#include "LModel.h"
#include "LInput.h"
#include "LFSMMgr.h"
#include "LPlayer.h"
#include "LSoundMgr.h"
#include <random>
#include <chrono>

class KObject;
class LNPC;

class NPCState
{
public:
	LNPC* m_pOwner = nullptr;
public:
	virtual bool Init() { return true; }
	virtual	void Process() = 0;
public:
	NPCState(LNPC* parent) : m_pOwner(parent) {}
	virtual ~NPCState() {}
};

class LNPC : public LSkinningModel
{
public:
	State m_CurrentState = State::NONE;
	LFiniteStateMachine* m_pFsm = nullptr;
	NPCState* m_pAction = nullptr;
	std::map<State, std::unique_ptr<NPCState>> m_pActionList;
public:
	bool IsFindPlayer = false;
	bool IsEndPatrol = false;
	bool IsDead = false;
	bool IsTakeDamage = false;
	bool IsAttackRange = false;
	bool IsMovable = true;
	bool IsRush = false;
	bool IsRushDir = true;
	bool IsUseRush = false;
	bool IsHeadShot = false;
	bool IsComboRange = false;
	bool IsOnAir = false;
	bool IsHitBladeAttack = false;
	bool IsHitPlayer = false;
	bool IsFirstRushDamage = true;
	bool IsTakeRushDamage = false;
	bool IsJumpAttackEnd = false;
	bool IsJumpAttackRange = false;
	
	LSound* m_ZombieSound = nullptr;
	float m_Speed = 50.0f;
	float m_RushSpeed = 400.0f;
	float m_ComboSpeed = 25.0f;
	float m_JumpAttackSpeed = 100.0f;
	TVector3 m_Dir;
	float m_PatrolRange = 10000.0f;
	float m_AttackRange = 25.0f;
	float m_ComboRange = 100.0f;
	float m_RushRange = 600.0f;
	float m_RushStart = 0.0f;
	float m_RushEnd = 2.0f;
	float m_RushCoolTimeStart = 0.0f;
	float m_RushCoolTimeEnd = 6.0f;
	float m_MapHeight = 0.0f;
	
	TVector3 m_Velocity;
	TVector3 m_RandomPos;
	LPlayer* m_Player;
	TVector3 m_PlayerPos;
	TVector3 m_NPCPos;
	TVector3 m_RushPos;
	TVector3 m_JumpVelocity = {-164.0f, 200.0f, 364.0f};
	std::default_random_engine m_Generator;
	std::uniform_int_distribution<int> m_Distribution;
	shared_ptr<KObject> m_enemyHp;
	shared_ptr<KObject> m_minimapMarker;
public:
	virtual State GetState() { return m_CurrentState; };
	virtual void FSM(FSMType fsmType) {};
	virtual void SetTransition(Event inputEvent) {};
	virtual void Move(TVector3 target) {};
	virtual void RushMove() {};
	virtual void ComboMove() {};
	virtual void JumpAttackMove(TVector3 target) {};
	virtual void MoveInstancing(TVector3 target) {};
	virtual int GetRandomNumber() { return 0; };
public:
	virtual	void Process() {};
	virtual bool Frame() override { return true; };
	virtual bool FrameInstancing() override { return true; };
	virtual bool Render() override { return true; };
	virtual bool RenderMark() { return true; };
public:
	LNPC() {};
	LNPC(LPlayer* player) : m_Distribution(-3000, 3000) {};
};

