#include "BossTrace.h"
#include "LGlobal.h"
#include "LFbxMgr.h"
#include "UIManager.h"

bool BossTrace::Init()
{
    return true;
}

void BossTrace::Process()
{
    if (m_pOwner->IsTakeDamage)
    {
        m_pOwner->SetTransition(Event::TAKEDAMAGE);
        return;
    }

    if (m_pOwner->IsUseRush)
    {
        m_pOwner->SetTransition(Event::BOSSJUMPATTACK);
        return;
    }

    if (m_pOwner->IsAttackRange)
    {
        m_pOwner->SetTransition(Event::BOSSSWIPING);
        return;
    }

    if (m_pOwner->IsMovable)
    {
        m_pOwner->Move(m_pOwner->m_PlayerPos);
    }

    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Boss_Run.fbx");
}

void BossTrace::Release()
{

}

BossTrace::BossTrace(Boss* parent) : NPCState(parent)
{

}

BossTrace::~BossTrace() {}