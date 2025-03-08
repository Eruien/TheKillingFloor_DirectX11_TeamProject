#include "BossDeath.h"
#include "LGlobal.h"
#include "LFbxMgr.h"
#include "UIManager.h"

bool BossDeath::Init()
{
    return true;
}

void BossDeath::Process()
{
    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Boss_Run.fbx");
}

void BossDeath::Release()
{

}

BossDeath::BossDeath(Boss* parent) : NPCState(parent)
{

}

BossDeath::~BossDeath() {}