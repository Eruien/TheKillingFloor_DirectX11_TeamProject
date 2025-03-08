#include "LScene.h"
#include "LGlobal.h"
#include "LMainScene.h"
#include "UIScene.h"
#include "MapScene.h"
#include "CharacterScene.h"
#include "EffectScene.h"

void LScene::FSM(FSMType fsmType)
{
    auto iter = LFSMMgr::GetInstance().m_map.find(fsmType);

    if (LFSMMgr::GetInstance().m_map.end() == iter)
    {
        MessageBoxA(NULL, "FSMType Error", "Error Box", MB_OK);
        return;
    }

    m_pFsm = iter->second.get();

    m_pActionList.insert(std::make_pair(State::MAINSCENE, std::make_unique<LMainScene>(this)));
    m_pActionList.insert(std::make_pair(State::UISCENE, std::make_unique<UIScene>(this)));
    m_pActionList.insert(std::make_pair(State::MAPSCENE, std::make_unique<MapScene>(this)));
    m_pActionList.insert(std::make_pair(State::CHARACTERSCENE, std::make_unique<CharacterScene>(this)));
    m_pActionList.insert(std::make_pair(State::EFFECTSCENE, std::make_unique<EffectScene>(this)));

    m_pAction = m_pActionList.find(State::MAINSCENE)->second.get();
    m_CurrentState = State::MAINSCENE;
}

void LScene::SetTransition(Event inputEvent)
{
    m_CurrentState = m_pFsm->StateTransition(m_CurrentState, inputEvent);
    m_pAction = m_pActionList.find(m_CurrentState)->second.get();
}

State LScene::GetState()
{
    return m_CurrentState;
}

void LScene::Process()
{
    m_pAction->Process();
}

bool LScene::Init()
{
    return true;
}

bool LScene::Frame()
{
    return true;
}

bool LScene::Render()
{
    return true;
}

bool LScene::Release()
{
    return true;
}

LScene::LScene() {}
LScene::~LScene() {}


