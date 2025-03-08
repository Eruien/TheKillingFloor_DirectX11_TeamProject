#include "EnemyPatrol.h"
#include "LGlobal.h"
#include "LFbxMgr.h"

bool EnemyPatrol::Init()
{
    return true;
}

void EnemyPatrol::Process()
{
    if (m_pOwner->IsTakeDamage)
    {
        m_pOwner->SetTransition(Event::TAKEDAMAGE);
        return;
    }

    if (m_pOwner->IsFindPlayer)
    {
        m_pOwner->SetTransition(Event::FINDPLAYER);
        return;
    }
    if (m_pOwner->IsMovable)
        m_pOwner->Move(m_pOwner->m_RandomPos);
    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Zombie_Walk_Lock.fbx");
}

void EnemyPatrol::Release()
{

}

EnemyPatrol::EnemyPatrol(Zombie* parent) : NPCState(parent)
{

}
EnemyPatrol::~EnemyPatrol() {}