#include "TankCombo.h"
#include "LGlobal.h"
#include "LFbxMgr.h"
#include "UIManager.h"

bool TankCombo::Init()
{
    return true;
}

void TankCombo::Process()
{
    if (m_pOwner->IsTakeDamage)
    {
        m_pOwner->SetTransition(Event::TAKEDAMAGE);
        return;
    }

    if (m_pOwner->m_TimerEnd)
    {
        m_pOwner->SetTransition(Event::ENDATTACK);
        return;
    }

    if (m_pOwner->IsMovable)
    {
        m_pOwner->ComboMove();
    }

    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Tank_Combo.fbx");
}

void TankCombo::Release()
{

}

TankCombo::TankCombo(Tank* parent) : NPCState(parent)
{

}
TankCombo::~TankCombo() {}