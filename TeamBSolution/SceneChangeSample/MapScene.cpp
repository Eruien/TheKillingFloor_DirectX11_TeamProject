#include "MapScene.h"
#include "LGlobal.h"
#include "LInput.h"
#include "LWrite.h"

bool MapScene::Init()
{
    return true;
}

void MapScene::Process()
{
    std::wstring textState = L"Map";
    //LWrite::GetInstance().AddText(textState, 320.0f, 500.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

    if (LInput::GetInstance().m_KeyState[DIK_ESCAPE] == DWORD(KEY_PUSH))
    {
        Release();
        m_pOwner->SetTransition(Event::GOMAINSCENE);
        return;
    }
}

void MapScene::Release()
{

}

MapScene::MapScene(LScene* parent) : SceneState(parent)
{

}
MapScene::~MapScene() {}