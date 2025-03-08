#include "PlayerIdle.h"
#include "LGlobal.h"
#include "LFbxMgr.h"

bool PlayerIdle::Init()
{
    return true;
}

void PlayerIdle::Process()
{
    if (m_pOwner->IsDeath)
    {
        m_pOwner->SetTransition(Event::FATALDAMAGE);
        return;
    }

    if (m_pOwner->IsTakeDammageAni)
    {
        m_pOwner->SetTransition(Event::TAKEDAMAGE);
        return;
    }

    if (m_pOwner->m_Type == PlayerType::GUN)
    {
        if (m_pOwner->IsReload)
        {
            m_pOwner->SetTransition(Event::STARTRELOAD);
            return;
        }
    }

    if (m_pOwner->IsRush)
    {
        m_pOwner->SetTransition(Event::USERUSH);
        return;
    }
   
    if (m_pOwner->IsAttack)
    {
        m_pOwner->SetTransition(Event::CLICKATTACKBUTTON);
        return;
    }

    if (m_pOwner->IsRun)
    {
        m_pOwner->SetTransition(Event::RUNSPEED);
        return;
    }

    if (m_pOwner->IsWalk)
    {
        m_pOwner->SetTransition(Event::WALKSPEED);
        return;
    }

   if (m_pOwner->m_CurrentGun == WeaponState::PISTOL)
   {
       m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Pistol_Idle.fbx");
   }
   else if (m_pOwner->m_CurrentGun == WeaponState::ASSAULTRIFLE)
   {
       m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Idle_Rifle_Ironsights.fbx");
   }
   else if (m_pOwner->m_CurrentGun == WeaponState::SHOTGUN)
   {
       m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Idle_Rifle_Ironsights.fbx");
   }
   else if (m_pOwner->m_CurrentGun == WeaponState::ONEHANDSWORD)
   {
       m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Idle_Rifle_Ironsights.fbx");
   }
   else if (m_pOwner->m_CurrentGun == WeaponState::TWOHANDSWORD)
   {
       m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"TwoHand_Idle_Anim.fbx");
   }

}

void PlayerIdle::Release()
{

}

PlayerIdle::PlayerIdle(LPlayer *parent) : PlayerState(parent)
{

}
PlayerIdle::~PlayerIdle() {}