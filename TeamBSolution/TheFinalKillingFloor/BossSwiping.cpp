#include "BossSwiping.h"
#include "LGlobal.h"
#include "LFbxMgr.h"

bool BossSwiping::Montage(int startFrame, int endFrame)
{
    if (startFrame <= m_pOwner->m_fCurrentAnimTime && endFrame >= m_pOwner->m_fCurrentAnimTime)
    {
        return true;
    }
    return false;
}

bool BossSwiping::Init()
{
    return true;
}

void BossSwiping::Process()
{
    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Boss_Swiping.fbx");
   
    if (Montage(50, 80))
    {
        m_pOwner->IsHitPlayer = true;
    }
    else
    {
        m_pOwner->IsHitPlayer = false;
    }

    if (m_pOwner->IsTakeDamage)
    {
        m_pOwner->SetTransition(Event::TAKEDAMAGE);
        return;
    }

    if (!m_pOwner->IsAttackRange && m_pOwner->m_TimerEnd)
    {
        m_pOwner->SetTransition(Event::PLAYEROUTATTACKRANGE);
        return;
    }
}

void BossSwiping::Release()
{

}

BossSwiping::BossSwiping(Boss* parent) : NPCState(parent)
{

}
BossSwiping::~BossSwiping() {}