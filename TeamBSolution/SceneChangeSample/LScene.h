#pragma once
#include "LFSMMgr.h"

class LScene;

class SceneState
{
public:
	LScene* m_pOwner = nullptr;
public:
	virtual bool Init() { return true; }
	virtual	void Process() = 0;
public:
	SceneState(LScene* parent) : m_pOwner(parent) {}
	virtual ~SceneState() {}
};

class LScene
{
public:
	State m_CurrentState = State::NONE;
	LFiniteStateMachine* m_pFsm = nullptr;
	SceneState* m_pAction = nullptr;
	std::map<State, std::unique_ptr<SceneState>> m_pActionList;
public:
	void FSM(FSMType fsmType);
	void SetTransition(Event inputEvent);
	State GetState();
public:
	virtual	void Process();
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	LScene();
	virtual ~LScene();
};

