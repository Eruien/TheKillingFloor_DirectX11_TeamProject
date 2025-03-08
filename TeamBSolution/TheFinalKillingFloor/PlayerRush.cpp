#include "PlayerRush.h"
#include "LGlobal.h"
#include "LFbxMgr.h"

bool PlayerRush::Init()
{
    return true;
}

void PlayerRush::Process()
{
    m_pOwner->IsMove = false;
  
    if (m_pOwner->m_TimerEnd)
    {
        m_pOwner->IsMove = true;
        m_pOwner->IsTakeDammageAni = false;
        m_pOwner->SetTransition(Event::ENDRUSH);
        return;
    }

    if (m_pOwner->m_CurrentGun == WeaponState::ONEHANDSWORD)
    {
        m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"OneHand_Rush.fbx");
    }
    else if (m_pOwner->m_CurrentGun == WeaponState::TWOHANDSWORD)
    {
        m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"OneHand_Rush.fbx");
    }
}

void PlayerRush::Release()
{

}

PlayerRush::PlayerRush(LPlayer* parent) : PlayerState(parent)
{

}
PlayerRush::~PlayerRush() {}