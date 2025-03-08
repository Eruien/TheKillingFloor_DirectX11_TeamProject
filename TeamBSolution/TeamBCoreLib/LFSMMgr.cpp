#include "LFSMMgr.h"

void LFiniteStateMachine::AddStateTransition(State keyState, Event inputEvent, State outputState)
{
	auto finiteIter = m_FiniteStateMap.find(keyState);

	if (m_FiniteStateMap.end() != finiteIter)
	{
		finiteIter->second->AddTransition(inputEvent, outputState);
	}
	else
	{
		m_FiniteStateMap.insert(std::make_pair(keyState, std::make_unique<LFiniteState>(keyState)));

		finiteIter = m_FiniteStateMap.find(keyState);
		finiteIter->second->AddTransition(inputEvent, outputState);
	}
}

State LFiniteStateMachine::StateTransition(State currentState, Event inputEvent)
{
	auto finiteIter = m_FiniteStateMap.find(currentState);

	if (finiteIter == m_FiniteStateMap.end())
	{
		return State::NONE;
	}

	auto retStateIter = finiteIter->second->m_StateMap.find(inputEvent);

	if (retStateIter == finiteIter->second->m_StateMap.end())
	{
		return State::NONE;
	}

	State state = retStateIter->second;
	return state;
}

// FSMMgr
LFiniteStateMachine* LFSMMgr::GetPtr(FSMType key)
{
	auto iter = m_map.find(key);

	if (m_map.end() == iter)
	{
		return nullptr;
	}

	return iter->second.get();
}

// 스마트 포인터 관련해서 오류날 소지가 있으니 조심 스마트 포인터를 move로 이전해서 참조로 넘겨준다.
bool LFSMMgr::Get(FSMType key, LFiniteStateMachine& ret)
{
	auto iter = m_map.find(key);

	if (m_map.end() == iter)
	{
		return false;
	}

	// 포인터 주소가 아니라 객체에 대한 주소가 필요하니깐 역참조 해서 move
	ret = std::move(*iter->second);

	return true;
}

bool LFSMMgr::Init()
{
	//Scene
	std::unique_ptr<LFiniteStateMachine> sceneFSM = std::make_unique<LFiniteStateMachine>();


	sceneFSM->AddStateTransition(State::LOADSCENE, Event::GOMAINSCENE, State::MAINSCENE);

	sceneFSM->AddStateTransition(State::MAINSCENE, Event::GOINGAMESCENE, State::INGAMESCENE);
	sceneFSM->AddStateTransition(State::MAINSCENE, Event::GOSELECTSCENE, State::SELECTSCENE);

	sceneFSM->AddStateTransition(State::SELECTSCENE, Event::GOINGAMESCENE, State::INGAMESCENE);

	sceneFSM->AddStateTransition(State::INGAMESCENE, Event::GOMAINSCENE, State::MAINSCENE);
	sceneFSM->AddStateTransition(State::INGAMESCENE, Event::GOENDSCENE, State::ENDSCENE);

	sceneFSM->AddStateTransition(State::ENDSCENE, Event::GOMAINSCENE, State::MAINSCENE);
	sceneFSM->AddStateTransition(State::ENDSCENE, Event::GOINGAMESCENE, State::INGAMESCENE);

	sceneFSM->AddStateTransition(State::INGAMESCENE, Event::GOSHOPSCENE, State::SHOPSCENE);
	sceneFSM->AddStateTransition(State::SHOPSCENE, Event::GOINGAMESCENE, State::INGAMESCENE);

	m_map.insert(std::make_pair(FSMType::SCENE, std::move(sceneFSM)));

	//Player
	std::unique_ptr<LFiniteStateMachine> PlayerFSM = std::make_unique<LFiniteStateMachine>();

	PlayerFSM->AddStateTransition(State::CHARACTERIDLE, Event::TAKEDAMAGE, State::CHARACTERTAKEDAMAGE);
	PlayerFSM->AddStateTransition(State::CHARACTERIDLE, Event::FATALDAMAGE, State::CHARACTERDEATH);
	PlayerFSM->AddStateTransition(State::CHARACTERIDLE, Event::STARTRELOAD, State::CHARACTERRELOAD);
	PlayerFSM->AddStateTransition(State::CHARACTERIDLE, Event::WALKSPEED, State::CHARACTERWALK);
	PlayerFSM->AddStateTransition(State::CHARACTERIDLE, Event::RUNSPEED, State::CHARACTERRUN);
	PlayerFSM->AddStateTransition(State::CHARACTERIDLE, Event::CLICKATTACKBUTTON, State::CHARACTERATTACK);
	PlayerFSM->AddStateTransition(State::CHARACTERIDLE, Event::USERUSH, State::CHARACTERRUSH);

	PlayerFSM->AddStateTransition(State::CHARACTERWALK, Event::TAKEDAMAGE, State::CHARACTERTAKEDAMAGE);
	PlayerFSM->AddStateTransition(State::CHARACTERWALK, Event::FATALDAMAGE, State::CHARACTERDEATH);
	PlayerFSM->AddStateTransition(State::CHARACTERWALK, Event::STARTRELOAD, State::CHARACTERRELOAD);
	PlayerFSM->AddStateTransition(State::CHARACTERWALK, Event::RUNSPEED, State::CHARACTERRUN);
	PlayerFSM->AddStateTransition(State::CHARACTERWALK, Event::CLICKATTACKBUTTON, State::CHARACTERATTACK);
	PlayerFSM->AddStateTransition(State::CHARACTERWALK, Event::IDLESPEED, State::CHARACTERIDLE);
	PlayerFSM->AddStateTransition(State::CHARACTERWALK, Event::USERUSH, State::CHARACTERRUSH);

	PlayerFSM->AddStateTransition(State::CHARACTERRUN, Event::TAKEDAMAGE, State::CHARACTERTAKEDAMAGE);
	PlayerFSM->AddStateTransition(State::CHARACTERRUN, Event::FATALDAMAGE, State::CHARACTERDEATH);
	PlayerFSM->AddStateTransition(State::CHARACTERRUN, Event::STARTRELOAD, State::CHARACTERRELOAD);
	PlayerFSM->AddStateTransition(State::CHARACTERRUN, Event::IDLESPEED, State::CHARACTERIDLE);
	PlayerFSM->AddStateTransition(State::CHARACTERRUN, Event::WALKSPEED, State::CHARACTERWALK);
	PlayerFSM->AddStateTransition(State::CHARACTERRUN, Event::CLICKATTACKBUTTON, State::CHARACTERATTACK);
	PlayerFSM->AddStateTransition(State::CHARACTERRUN, Event::USERUSH, State::CHARACTERRUSH);

	PlayerFSM->AddStateTransition(State::CHARACTERATTACK, Event::GUNSHOOT, State::CHARACTERSHOOT);
	PlayerFSM->AddStateTransition(State::CHARACTERATTACK, Event::BLADEONESLASH, State::CHARACTERONEHANDSLASH);
	PlayerFSM->AddStateTransition(State::CHARACTERATTACK, Event::BLADETWOSLASH, State::CHARACTERTWOHANDSLASH);

	PlayerFSM->AddStateTransition(State::CHARACTERSHOOT, Event::TAKEDAMAGE, State::CHARACTERTAKEDAMAGE);
	PlayerFSM->AddStateTransition(State::CHARACTERSHOOT, Event::FATALDAMAGE, State::CHARACTERDEATH);
	PlayerFSM->AddStateTransition(State::CHARACTERSHOOT, Event::ENDATTACK, State::CHARACTERIDLE);
	PlayerFSM->AddStateTransition(State::CHARACTERSHOOT, Event::STARTRELOAD, State::CHARACTERRELOAD);

	PlayerFSM->AddStateTransition(State::CHARACTERONEHANDSLASH, Event::FATALDAMAGE, State::CHARACTERDEATH);
	PlayerFSM->AddStateTransition(State::CHARACTERONEHANDSLASH, Event::ENDATTACK, State::CHARACTERIDLE);
	PlayerFSM->AddStateTransition(State::CHARACTERONEHANDSLASH, Event::USERUSH, State::CHARACTERRUSH);

	PlayerFSM->AddStateTransition(State::CHARACTERTWOHANDSLASH, Event::FATALDAMAGE, State::CHARACTERDEATH);
	PlayerFSM->AddStateTransition(State::CHARACTERTWOHANDSLASH, Event::ENDATTACK, State::CHARACTERIDLE);
	PlayerFSM->AddStateTransition(State::CHARACTERTWOHANDSLASH, Event::USERUSH, State::CHARACTERRUSH);

	PlayerFSM->AddStateTransition(State::CHARACTERRUSH, Event::ENDRUSH, State::CHARACTERIDLE);

	PlayerFSM->AddStateTransition(State::CHARACTERRELOAD, Event::FATALDAMAGE, State::CHARACTERDEATH);
	PlayerFSM->AddStateTransition(State::CHARACTERRELOAD, Event::ENDRELOAD, State::CHARACTERIDLE);

	PlayerFSM->AddStateTransition(State::CHARACTERTAKEDAMAGE, Event::FATALDAMAGE, State::CHARACTERDEATH);
	PlayerFSM->AddStateTransition(State::CHARACTERTAKEDAMAGE, Event::RECOVERYDAMAGE, State::CHARACTERIDLE);

	PlayerFSM->AddStateTransition(State::CHARACTERDEATH, Event::CHARACTERREVIVE, State::CHARACTERIDLE);

	m_map.insert(std::make_pair(FSMType::PLAYER, std::move(PlayerFSM)));

	//Enemy
	std::unique_ptr<LFiniteStateMachine> EnemyFSM = std::make_unique<LFiniteStateMachine>();

	EnemyFSM->AddStateTransition(State::ENEMYPATROL, Event::FINDPLAYER, State::ENEMYTRACE);
	EnemyFSM->AddStateTransition(State::ENEMYPATROL, Event::FATALDAMAGE, State::ENEMYDEATH);
	EnemyFSM->AddStateTransition(State::ENEMYPATROL, Event::TAKEDAMAGE, State::ENEMYTAKEDAMAGE);

	EnemyFSM->AddStateTransition(State::ENEMYTRACE, Event::FATALDAMAGE, State::ENEMYDEATH);
	EnemyFSM->AddStateTransition(State::ENEMYTRACE, Event::TAKEDAMAGE, State::ENEMYTAKEDAMAGE);
	EnemyFSM->AddStateTransition(State::ENEMYTRACE, Event::PLAYERINATTACKRANGE, State::ENEMYATTACK);

	EnemyFSM->AddStateTransition(State::ENEMYATTACK, Event::TAKEDAMAGE, State::ENEMYTAKEDAMAGE);
	EnemyFSM->AddStateTransition(State::ENEMYATTACK, Event::PLAYEROUTATTACKRANGE, State::ENEMYPATROL);

	EnemyFSM->AddStateTransition(State::ENEMYTAKEDAMAGE, Event::RECOVERYDAMAGE, State::ENEMYPATROL);
	EnemyFSM->AddStateTransition(State::ENEMYTAKEDAMAGE, Event::FATALDAMAGE, State::ENEMYDEATH);

	m_map.insert(std::make_pair(FSMType::ENEMY, std::move(EnemyFSM)));

	//Tank
	std::unique_ptr<LFiniteStateMachine> TankFSM = std::make_unique<LFiniteStateMachine>();

	TankFSM->AddStateTransition(State::TANKTRACE, Event::TAKEDAMAGE, State::TANKTAKEDAMAGE);
	TankFSM->AddStateTransition(State::TANKTRACE, Event::FATALDAMAGE, State::TANKDEATH);
	TankFSM->AddStateTransition(State::TANKTRACE, Event::TANKRUSHATTACK, State::TANKRUSH);
	//TankFSM->AddStateTransition(State::TANKTRACE, Event::TANKCOMBOATTACK, State::TANKCOMBO);

	/*TankFSM->AddStateTransition(State::TANKCOMBO, Event::TAKEDAMAGE, State::TANKTAKEDAMAGE);
	TankFSM->AddStateTransition(State::TANKCOMBO, Event::FATALDAMAGE, State::TANKDEATH);
	TankFSM->AddStateTransition(State::TANKCOMBO, Event::ENDATTACK, State::TANKTRACE);*/

	TankFSM->AddStateTransition(State::TANKRUSH, Event::FATALDAMAGE, State::TANKDEATH);
	TankFSM->AddStateTransition(State::TANKRUSH, Event::ENDATTACK, State::TANKTRACE);

	TankFSM->AddStateTransition(State::TANKTAKEDAMAGE, Event::FATALDAMAGE, State::TANKDEATH);
	TankFSM->AddStateTransition(State::TANKTAKEDAMAGE, Event::RECOVERYDAMAGE, State::TANKTRACE);

	m_map.insert(std::make_pair(FSMType::TANK, std::move(TankFSM)));

	//Boss
	std::unique_ptr<LFiniteStateMachine> BossFSM = std::make_unique<LFiniteStateMachine>();

	BossFSM->AddStateTransition(State::BOSSTRACE, Event::TAKEDAMAGE, State::BOSSTAKEDAMAGE);
	BossFSM->AddStateTransition(State::BOSSTRACE, Event::FATALDAMAGE, State::BOSSDEATH);
	BossFSM->AddStateTransition(State::BOSSTRACE, Event::BOSSSWIPING, State::BOSSSWIPING);
	BossFSM->AddStateTransition(State::BOSSTRACE, Event::BOSSJUMPATTACK, State::BOSSJUMPATTACK);
	
	BossFSM->AddStateTransition(State::BOSSTAKEDAMAGE, Event::FATALDAMAGE, State::BOSSDEATH);
	BossFSM->AddStateTransition(State::BOSSTAKEDAMAGE, Event::RECOVERYDAMAGE, State::BOSSTRACE);

	BossFSM->AddStateTransition(State::BOSSSWIPING, Event::TAKEDAMAGE, State::BOSSTAKEDAMAGE);
	BossFSM->AddStateTransition(State::BOSSSWIPING, Event::PLAYEROUTATTACKRANGE, State::BOSSTRACE);

	BossFSM->AddStateTransition(State::BOSSJUMPATTACK, Event::ENDATTACK, State::BOSSTRACE);
	BossFSM->AddStateTransition(State::BOSSJUMPATTACK, Event::TAKEDAMAGE, State::BOSSTAKEDAMAGE);

	m_map.insert(std::make_pair(FSMType::BOSS, std::move(BossFSM)));

	return true;
}

bool LFSMMgr::Release()
{
	m_map.clear();

	return true;
}

LFSMMgr::LFSMMgr()
{
	Init();
}
LFSMMgr::~LFSMMgr() {}


