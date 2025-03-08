#include "TankTakeDamage.h"
#include "LGlobal.h"
#include "LFbxMgr.h"
#include "KObject.h"

bool TankTakeDamage::Init()
{
    return true;
}

void TankTakeDamage::Process()
{
   /* if (!m_Timer && LGlobal::g_BulletCount > 0)
    {
        m_pOwner->m_HP -= 5.0f;
        m_pOwner->IsTakeDamage = false;
        m_pOwner->m_TimerStart = true;
        m_Timer = true;
    }*/

    if (m_pOwner->IsTakeDamage)
    {
        if (LGlobal::g_PlayerModel->m_Type == PlayerType::GUN)
        {
            if (m_pOwner->IsHeadShot)
            {
                LSoundMgr::GetInstance().GetPtr(L"headshot.mp3")->PlayEffect();
                m_pOwner->m_HP -= LGlobal::g_PlayerModel->m_Gun->m_GunSpec.Damage * m_HeadShootRate * m_GunDamageRate;
            }
            else
            {
                m_pOwner->m_HP -= LGlobal::g_PlayerModel->m_Gun->m_GunSpec.Damage * m_GunDamageRate;
            }
        }
        else if (LGlobal::g_PlayerModel->m_Type == PlayerType::SWORD)
        {
            if (m_pOwner->IsTakeRushDamage)
            {
                m_pOwner->IsTakeRushDamage = false;
                m_pOwner->m_HP -= LGlobal::g_PlayerModel->m_Gun->m_SwordSpec.RushDamage;
            }
            else
            {
                LSoundMgr::GetInstance().GetPtr(L"BladeHitSound.mp3")->Play(false);
                m_pOwner->m_HP -= LGlobal::g_PlayerModel->m_Gun->m_SwordSpec.Damage * m_SwordDamageRate;
            }
        }
       
        UpdateHPbar();
        m_pOwner->IsTakeDamage = false;
    }

    if (m_pOwner->m_HP < m_AnimationHP)
    {
        IsTakeDamageAnimation = true;
        m_AnimationHP -= m_MinusHP;
    }

    if (m_pOwner->IsDead)
    {
        m_pOwner->SetTransition(Event::FATALDAMAGE);
        return;
    }

    if (IsTakeDamageAnimation)
    {
        m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Tank_TakeDamage.fbx");

        if (m_pOwner->m_TimerEnd)
        {
            IsTakeDamageAnimation = false;
            m_pOwner->IsTakeDamage = false;
            m_pOwner->SetTransition(Event::RECOVERYDAMAGE);
            return;
        }
    }
    else
    {
        m_pOwner->IsTakeDamage = false;
        m_pOwner->SetTransition(Event::RECOVERYDAMAGE);
        return;
    }
}

void TankTakeDamage::Release()
{

}

void TankTakeDamage::UpdateHPbar()
{
    float hp = LGlobal::g_PlayerModel->m_HP;

    m_pOwner->m_enemyHp->m_VertexList[1].p.x = 0.5f - (1 - m_pOwner->m_HP / 100);
    m_pOwner->m_enemyHp->m_VertexList[4].p.x = 0.5f - (1 - m_pOwner->m_HP / 100);
    m_pOwner->m_enemyHp->m_VertexList[5].p.x = 0.5f - (1 - m_pOwner->m_HP / 100);

    D3D11_BUFFER_DESC bufferDesc = { 0, };
    bufferDesc.ByteWidth = sizeof(SimpleVertex) * m_pOwner->m_enemyHp->m_VertexList.size();
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initialData = { 0, };
    initialData.pSysMem = &m_pOwner->m_enemyHp->m_VertexList.at(0);

    HRESULT hr = m_pOwner->m_enemyHp->m_pDevice->CreateBuffer(
        &bufferDesc,
        &initialData,
        &m_pOwner->m_enemyHp->m_pVertexBuffer);

    if (FAILED(hr))
    {
        MessageBoxA(NULL, "Create Buffer Error", "Error Box", MB_OK);
        return;
    }
}

TankTakeDamage::TankTakeDamage(Tank* parent) : NPCState(parent)
{

}
TankTakeDamage::~TankTakeDamage() {}