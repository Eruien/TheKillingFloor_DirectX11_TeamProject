#include "EnemyDeath.h"
#include "LGlobal.h"
#include "LFbxMgr.h"
#include "UIManager.h"
bool EnemyDeath::Init()
{
    return true;
}

void EnemyDeath::Process()
{
    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Zombie_Death.fbx");
    m_pOwner->IsDead = true;

}

void EnemyDeath::Release()
{

}

EnemyDeath::EnemyDeath(Zombie* parent) : NPCState(parent)
{

}
EnemyDeath::~EnemyDeath() {}