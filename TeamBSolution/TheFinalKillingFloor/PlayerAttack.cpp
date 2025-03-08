#include "PlayerAttack.h"

bool PlayerAttack::Init()
{
    return true;
}

void PlayerAttack::Process()
{
    if (m_pOwner->m_Type == PlayerType::GUN)
    {
        m_pOwner->SetTransition(Event::GUNSHOOT);
    }
    else if (m_pOwner->m_Type == PlayerType::SWORD)
    {
        if (m_pOwner->m_CurrentGun == WeaponState::ONEHANDSWORD)
        {
            m_pOwner->SetTransition(Event::BLADEONESLASH);
        }
        else if (m_pOwner->m_CurrentGun == WeaponState::TWOHANDSWORD)
        {
            m_pOwner->SetTransition(Event::BLADETWOSLASH);
        }
    }
}

void PlayerAttack::Release()
{

}

PlayerAttack::PlayerAttack(LPlayer* parent) : PlayerState(parent)
{

}
PlayerAttack::~PlayerAttack() {}