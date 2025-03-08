#include "EndScene.h"
#include "LGlobal.h"
#include "LInput.h"
#include "LWrite.h"
#include "UIManager.h"
#include "LPlayer.h"
bool EndScene::Init()
{
    if (LGlobal::g_PlayerModel->IsDeath)
    {
        UIManager::GetInstance().GetUIObject(L"Eng_Bg")->GetScript<ChangeTexture>(L"ChangeTexture")->ChangeFromPath(L"../../res/ui/End2.png");
    }
    else 
    {
        UIManager::GetInstance().GetUIObject(L"Eng_Bg")->GetScript<ChangeTexture>(L"ChangeTexture")->ChangeFromPath(L"../../res/ui/End1.png");
    }
    
    return true;
}

void EndScene::Render()
{
    UIManager::GetInstance().Render();
}

void EndScene::Process()
{
    std::wstring textState = L"EndScene";
    //LWrite::GetInstance().AddText(textState, 320.0f, 500.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

    if (LInput::GetInstance().m_KeyStateOld[DIK_ESCAPE] == KEY_PUSH)
    {
        Release();
        LScene::GetInstance().SetTransition(Event::GOMAINSCENE);
        return;
    }

    if (LInput::GetInstance().m_KeyStateOld[DIK_ESCAPE] == KEY_PUSH)
    {
        Release();
        LScene::GetInstance().SetTransition(Event::GOINGAMESCENE);
        return;
    }
    UIManager::GetInstance().Frame();
}

void EndScene::Release()
{

}

EndScene::EndScene(LScene* parent) : SceneState(parent)
{

}
EndScene::~EndScene() {}