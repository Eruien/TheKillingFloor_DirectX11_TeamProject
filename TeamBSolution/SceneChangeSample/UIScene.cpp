#include "UIScene.h"
#include "LGlobal.h"
#include "LInput.h"
#include "LWrite.h"

bool UIScene::Init()
{
    return true;
}

void UIScene::Process()
{
    std::wstring textState = L"UI";
    LWrite::GetInstance().AddText(textState, 320.0f, 500.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

    if (LInput::GetInstance().m_KeyState[DIK_ESCAPE] == DWORD(KEY_PUSH))
    {
        Release();
        m_pOwner->SetTransition(Event::GOMAINSCENE);
        return;
    }
}

void UIScene::Release()
{
  
}

UIScene::UIScene(LScene* parent) : SceneState(parent)
{

}
UIScene::~UIScene() {}