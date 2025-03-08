#include "PlayerDeath.h"
#include "LGlobal.h"
#include "LFbxMgr.h"

bool PlayerDeath::Init()
{
    m_DeathSound = LSoundMgr::GetInstance().Load(L"../../res/sound/Dead.WAV");
    return true;
}

void PlayerDeath::Process()
{
    m_DeathSound->Play(false);
    m_pOwner->IsMove = false;

    if (m_pOwner->m_TimerEnd)
    {
        m_pOwner->IsMove = true;
        m_pOwner->IsDeath = false;
        m_pOwner->m_HP = 100.0f;
        m_pOwner->SetTransition(Event::CHARACTERREVIVE);
        return;
    }

    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Player_Death.fbx");
}

void PlayerDeath::Release()
{

}

PlayerDeath::PlayerDeath(LPlayer* parent) : PlayerState(parent)
{
    Init();
}
PlayerDeath::~PlayerDeath() {}