#include "EffectScene.h"
#include "LGlobal.h"
#include "LInput.h"
#include "LWrite.h"

bool EffectScene::Init()
{
    return true;
}

void EffectScene::Process()
{
    std::wstring textState = L"Effect";
    LWrite::GetInstance().AddText(textState, 320.0f, 500.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

    if (LInput::GetInstance().m_KeyState[DIK_ESCAPE] == DWORD(KEY_PUSH))
    {
        Release();
        m_pOwner->SetTransition(Event::GOMAINSCENE);
        return;
    }
}

void EffectScene::Release()
{

}

EffectScene::EffectScene(LScene* parent) : SceneState(parent)
{

}
EffectScene::~EffectScene() {}