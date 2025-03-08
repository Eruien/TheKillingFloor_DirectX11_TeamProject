#pragma once
#include "LStd.h"
#include "LModel.h"
#include "LInput.h"
#include "LFSMMgr.h"
#include "LWeaponMgr.h"

class LPlayer;

class PlayerState
{
public:
	LPlayer* m_pOwner = nullptr;
public:
	virtual bool Init() { return true; }
	virtual	void Process() = 0;
public:
	PlayerState(LPlayer* parent) : m_pOwner(parent) {}
	virtual ~PlayerState() {}
};

class LPlayer : public LSkinningModel
{
public:
	State m_CurrentState = State::NONE;
	WeaponState m_CurrentGun = WeaponState::PISTOL;
	ComboType m_ComboType = ComboType::NONE;
	int m_CurrentTrailFrame = 0;
	LFiniteStateMachine* m_pFsm = nullptr;
	PlayerState* m_pAction = nullptr;
	LWeapon* m_Gun;
	std::map<State, std::unique_ptr<PlayerState>> m_pActionList;
public:
	bool IsWalk = false;
	bool IsRun = false;
	bool IsAttack = false;
	bool IsMove = true;
	bool IsReload = false;
	bool IsEndReload = true;
	bool IsShoot = false;
	bool IsSlash = false;
	bool IsMoveOneDir = false;
	bool IsTakeDamage = false;
	bool IsInvincibility = true;
	bool IsDeath = false;
	bool IsTakeDammageAni = false;
	bool IsSteamPack = false;
	bool IsZedTime = false;
	bool IsOnAir = false;
	bool IsKillable = true;
	bool IsResetBladeAttack = false;
	bool IsRush = false;
	bool IsUseRush = true;

	float accumulatedTime = 0.0f;
public:
	enum BACKMOVESPEED {
		BACKMOVESPEED = 80
	};
	enum FRONTMOVESPEED {
		FRONTMOVESPEED = 100
	};
	enum RUSHSPEED {
		RUSHSPEED = 400
	};
	enum RUNMOVESPEED {
		RUNMOVESPEED = 150
	};

	int m_ZedTimeCount = 1;
	float m_Speed = 0.0f;
	float m_StartShoot = 0.0f;
	float m_StartTakeDamage = 0.0f;
	float m_EndTakeDamage = 1.0f;
	float m_SteamPackStart = 0.0f;
	float m_SteamPackEnd = 5.0f;
	float m_ZedTimeStart = 0.0f;
	float m_ZedTimeEnd = 5.0f;
	float m_RushStart = 0.0f;
	float m_RushEnd = 3.0f;
	float m_RushCoolTimeStart = 0.0f;
	float m_RushCoolTimeEnd = 5.0f;
	int m_Money = 0;
	TVector3 m_AddDirection;
	TVector3 m_Velocity;
	TMatrix m_matForAnim;
	// 충돌 처리를 위한 직전 위치. 시진 추가
	TVector3 m_PrevPosition;
public:
	void FSM(FSMType fsmType);
	void SetTransition(Event inputEvent);
	State GetState();
	void Move();
	void RushMove();
	void ItemChnge(WeaponState gun, int index);
public:
	virtual	void Process();
	bool Frame() override;
	bool GunFrame();
	bool SwordFrame();

	bool Render() override;
	bool PostFrame();
public:
	//LPlayer();
};

