#pragma once
#include "LFiniteState.h"

enum class FSMType
{
	SCENE,
	PLAYER,
	ENEMY,
	TANK,
	BOSS,
};

class LFiniteStateMachine
{
public:
	std::map<State, std::unique_ptr<LFiniteState>> m_FiniteStateMap;
public:
	void AddStateTransition(State keyState, Event inputEvent, State outputState);
	State StateTransition(State currentState, Event inputEvent);
};

class LFSMMgr
{
	using LList = std::map<FSMType, std::unique_ptr<LFiniteStateMachine>>;
public:
	static LFSMMgr& GetInstance()
	{
		static LFSMMgr input;
		return input;
	}
	LList m_map;
public:
	LFiniteStateMachine* GetPtr(FSMType key);
	bool Get(FSMType key, LFiniteStateMachine& ret);
public:
	bool Init();
	bool Release();
private:
	LFSMMgr();
public:
	virtual ~LFSMMgr();
};

