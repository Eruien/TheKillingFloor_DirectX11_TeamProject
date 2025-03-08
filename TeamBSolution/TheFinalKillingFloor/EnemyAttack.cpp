#include "EnemyAttack.h"
#include "LGlobal.h"
#include "LFbxMgr.h"

bool EnemyAttack::Montage(int startFrame, int endFrame)
{
    if ((m_pOwner->m_pActionModel->m_iStartFrame + startFrame) <= m_pOwner->m_fCurrentAnimTime
        && (m_pOwner->m_pActionModel->m_iEndFrame + endFrame) >= m_pOwner->m_fCurrentAnimTime)
    {
        return true;
    }
    return false;
}

bool EnemyAttack::Init()
{
    return true;
}

void EnemyAttack::Process()
{
    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Zombie_Attack_Anim.fbx");

    if (Montage(44, 80))
    {
        m_pOwner->IsHitPlayer = true;
    }
    else
    {
        m_pOwner->IsHitPlayer = false;
    }

    if (m_pOwner->IsTakeDamage)
    {
        m_Timer = false;
        m_pOwner->SetTransition(Event::TAKEDAMAGE);
        return;
    }

    if (!m_pOwner->IsAttackRange && m_pOwner->m_TimerEnd)
    {
        m_Timer = false;
        m_pOwner->SetTransition(Event::PLAYEROUTATTACKRANGE);
        return;
    }

    if (!m_Timer)
    {
        m_pOwner->m_TimerStart = true;
        m_Timer = true;
    }
}

void EnemyAttack::Release()
{

}

EnemyAttack::EnemyAttack(Zombie* parent) : NPCState(parent)
{

}
EnemyAttack::~EnemyAttack() {}