#include "PlayerGunShoot.h"
#include "LGlobal.h"
#include "LFbxMgr.h"
#include "LInput.h"

bool PlayerGunShoot::Montage(int startFrame, int endFrame)
{
    if (startFrame <= m_pOwner->m_fCurrentAnimTime && endFrame >= m_pOwner->m_fCurrentAnimTime)
    {
        return true;
    }
    return false;
}

bool PlayerGunShoot::Init()
{
    return true;
}

void PlayerGunShoot::Process()
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

    if (m_pOwner->IsReload)
    {
        m_pOwner->SetTransition(Event::STARTRELOAD);
        return;
    }

    if (!m_pOwner->IsAttack)
    {
        m_pOwner->SetTransition(Event::ENDATTACK);
        return;
    }
   
    if (m_pOwner->m_CurrentGun == WeaponState::PISTOL)
    {
        //m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Pistol_Shoot.fbx");
        m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Pistol_Idle.fbx");
    }
    else if (m_pOwner->m_CurrentGun == WeaponState::ASSAULTRIFLE)
    {
        m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Fire_Rifle_Ironsights.fbx");
    }
    else if (m_pOwner->m_CurrentGun == WeaponState::SHOTGUN)
    {
        m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Shotgun_Fire.fbx");
    }
}

void PlayerGunShoot::Release()
{

}

PlayerGunShoot::PlayerGunShoot(LPlayer* parent) : PlayerState(parent)
{

}
PlayerGunShoot::~PlayerGunShoot() {}