#include "TankTrace.h"
#include "LGlobal.h"
#include "LFbxMgr.h"
#include "UIManager.h"

bool TankTrace::Init()
{
    return true;
}

void TankTrace::Process()
{
    if (m_pOwner->IsTakeDamage)
    {
        m_pOwner->SetTransition(Event::TAKEDAMAGE);
        return;
    }

    if (m_pOwner->IsRush)
    {
        m_pOwner->SetTransition(Event::TANKRUSHATTACK);
        return;
    }

   /* if (m_pOwner->IsComboRange)
    {
        m_pOwner->SetTransition(Event::TANKCOMBOATTACK);
        return;
    }*/

    if (m_pOwner->IsMovable)
    {
        m_pOwner->Move(m_pOwner->m_PlayerPos);
    }

    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Tank_Walk.fbx");
}

void TankTrace::Release()
{

}

TankTrace::TankTrace(Tank* parent) : NPCState(parent)
{

}

TankTrace::~TankTrace() {}