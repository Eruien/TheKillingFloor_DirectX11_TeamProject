#include "EnemyTrace.h"
#include "LGlobal.h"
#include "LFbxMgr.h"

bool EnemyTrace::Init()
{
    return true;
}

void EnemyTrace::Process()
{
    if (m_pOwner->IsTakeDamage)
    {
        m_pOwner->SetTransition(Event::TAKEDAMAGE);
        return;
    }

    if (m_pOwner->IsAttackRange)
    {
        m_pOwner->SetTransition(Event::PLAYERINATTACKRANGE);
        return;
    }
    if (m_pOwner->IsMovable)
    {
        m_pOwner->Move(m_pOwner->m_PlayerPos);
    }
    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Zombie_Walk_Lock.fbx");
}

void EnemyTrace::Release()
{

}

EnemyTrace::EnemyTrace(Zombie* parent) : NPCState(parent)
{

}
EnemyTrace::~EnemyTrace() {}