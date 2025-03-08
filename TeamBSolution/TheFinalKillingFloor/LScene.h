#pragma once
#include "LFSMMgr.h"
#include <thread>

class LScene;

class SceneState
{
public:
	LScene* m_pOwner = nullptr;
public:
	virtual bool Init() { return true; }
	virtual	void Process() = 0;
	virtual	void Render() = 0;
	virtual void Retry() {};
public:
	SceneState(LScene* parent) : m_pOwner(parent) {}
	virtual ~SceneState() {}
};

class LScene
{
public:
	static LScene& GetInstance()
	{
		static LScene instance;
		return instance;
	}
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
	virtual	void Render();
	bool Init();
	bool Frame();
	bool Release();

	void InitThread();
public:
	LScene();
	virtual ~LScene();
};

