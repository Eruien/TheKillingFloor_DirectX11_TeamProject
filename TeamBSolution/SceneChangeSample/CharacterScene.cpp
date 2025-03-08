#include "CharacterScene.h"
#include "LGlobal.h"
#include "LInput.h"
#include "LWrite.h"

bool CharacterScene::Init()
{
    return true;
}

void CharacterScene::Process()
{
    std::wstring textState = L"Character";
    LWrite::GetInstance().AddText(textState, 320.0f, 500.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

    if (LInput::GetInstance().m_KeyState[DIK_ESCAPE] == KEY_PUSH)    {
        Release();
        m_pOwner->SetTransition(Event::GOMAINSCENE);
        return;
    }
}

void CharacterScene::Release()
{

}

CharacterScene::CharacterScene(LScene* parent) : SceneState(parent)
{

}
CharacterScene::~CharacterScene() {}