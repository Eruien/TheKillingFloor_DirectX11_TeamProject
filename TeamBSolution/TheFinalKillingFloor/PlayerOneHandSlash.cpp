#include "PlayerOneHandSlash.h"
#include "LGlobal.h"
#include "LFbxMgr.h"
#include "LInput.h"

bool PlayerOneHandSlash::Montage(int startFrame, int endFrame)
{
    if (startFrame <= m_pOwner->m_fCurrentAnimTime && endFrame >= m_pOwner->m_fCurrentAnimTime)
    {
        return true;
    }
    return false;
}

bool PlayerOneHandSlash::Init()
{
    return true;
}

void PlayerOneHandSlash::Process()
{
    m_pOwner->m_AnimationRate = m_pOwner->m_Gun->m_SwordSpec.SlashSpeed;

    if (m_pOwner->IsRush)
    {
        m_pOwner->IsResetBladeAttack = true;
        IsFirstClick = false;
        IsClick = false;
        m_CurrentCombo = ComboType::INWARD;
        m_pOwner->IsTakeDammageAni = false;
        IsFirstSlashSound = true;
        IsSecondSlashSound = true;
        m_pOwner->m_AnimationRate = 1.0f;
        m_pOwner->SetTransition(Event::USERUSH);
        return;
    }

    if (m_CurrentCombo == ComboType::OUTWARD)
    {
        if (Montage(20, 38))
        {
            if (IsSecondSlashSound)
            {
                m_pOwner->m_ComboType = ComboType::OUTWARD;
                LSoundMgr::GetInstance().GetPtr(L"OneHandSlashTwo.WAV")->Play(false);
                IsSecondSlashSound = false;
            }
            m_pOwner->IsSlash = true;
        }
        else
        {
            m_pOwner->m_ComboType = ComboType::NONE;
            m_pOwner->IsSlash = false;
        }
    }
    else if (m_CurrentCombo == ComboType::INWARD)
    {
        if (Montage(32, 38))
        {
            if (IsFirstSlashSound)
            {
                m_pOwner->m_ComboType = ComboType::INWARD;
                LSoundMgr::GetInstance().GetPtr(L"OneHandSlashOne.WAV")->Play(false);
                IsFirstSlashSound = false;
            }
            m_pOwner->IsSlash = true;
        }
        else
        {
            m_pOwner->m_ComboType = ComboType::NONE;
            m_pOwner->IsSlash = false;
        }
    }

    if (LInput::GetInstance().m_MouseState[0] == KEY_PUSH && !IsFirstClick)
    {
        IsFirstClick = true;
        IsClick = true;
    }
    
    if (IsClick && m_pOwner->m_TimerEnd)
    {
        m_pOwner->IsResetBladeAttack = true;
        m_pOwner->m_TimerEnd = false;
        IsClick = false;
        m_CurrentCombo = ComboType::OUTWARD;
    }

    if (m_pOwner->IsDeath)
    {
        m_pOwner->IsResetBladeAttack = true;
        IsFirstClick = false;
        IsClick = false;
        m_CurrentCombo = ComboType::INWARD;
        m_pOwner->IsTakeDammageAni = false;
        IsFirstSlashSound = true;
        IsSecondSlashSound = true;
        m_pOwner->m_AnimationRate = 1.0f;
        m_pOwner->SetTransition(Event::FATALDAMAGE);
        return;
    }

    if (m_pOwner->m_TimerEnd)
    {
        m_pOwner->IsResetBladeAttack = true;
        IsFirstClick = false;
        IsClick = false;
        m_CurrentCombo = ComboType::INWARD;
        m_pOwner->IsTakeDammageAni = false;
        IsFirstSlashSound = true;
        IsSecondSlashSound = true;
        m_pOwner->m_AnimationRate = 1.0f;
        m_pOwner->SetTransition(Event::ENDATTACK);
        return;
    }
  
    if (m_CurrentCombo == ComboType::INWARD)
    {
        m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"OneHand_Inward.fbx");
    }
    else if (m_CurrentCombo == ComboType::OUTWARD)
    {
        m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"OneHand_Outward.fbx");
    }
}

void PlayerOneHandSlash::Release()
{

}

PlayerOneHandSlash::PlayerOneHandSlash(LPlayer* parent) : PlayerState(parent)
{

}
PlayerOneHandSlash::~PlayerOneHandSlash() {}