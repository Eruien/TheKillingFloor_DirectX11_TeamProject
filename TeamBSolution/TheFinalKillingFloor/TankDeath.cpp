#include "TankDeath.h"
#include "LGlobal.h"
#include "LFbxMgr.h"
#include "UIManager.h"
#include "LPlayer.h"

bool TankDeath::Init()
{
    return true;
}

void TankDeath::Process()
{
    if (m_pOwner->m_TimerEnd)
    {
        m_pOwner->IsDead = true;
    }
    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Tank_Death.fbx");
 
}

void TankDeath::Release()
{

}

TankDeath::TankDeath(Tank* parent) : NPCState(parent)
{

}
TankDeath::~TankDeath() {}