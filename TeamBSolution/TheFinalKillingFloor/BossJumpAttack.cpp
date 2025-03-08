#include "BossJumpAttack.h"
#include "LGlobal.h"
#include "LFbxMgr.h"
#include "UIManager.h"

bool BossJumpAttack::Init()
{
    return true;
}

void BossJumpAttack::Process()
{
    m_pOwner->IsHitPlayer = true;

    if (m_pOwner->IsOnAir == false)
    {
        m_pOwner->IsOnAir = true;
        TVector3 vNormal = m_pOwner->m_matControl.Forward();
        vNormal.Normalize();
        vNormal.y = 1.5f;
        m_pOwner->m_Velocity = vNormal * 200;
    }

    if (m_pOwner->IsJumpAttackEnd)
    {
        m_pOwner->IsJumpAttackEnd = false;
        m_pOwner->IsHitPlayer = false;
        m_pOwner->IsOnAir = false;
        m_pOwner->IsUseRush = false;
        m_pOwner->SetTransition(Event::ENDATTACK);
        return;
    }

    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Boss_JumpAttack.fbx");
}

void BossJumpAttack::Release()
{

}

BossJumpAttack::BossJumpAttack(Boss* parent) : NPCState(parent)
{

}

BossJumpAttack::~BossJumpAttack() {}