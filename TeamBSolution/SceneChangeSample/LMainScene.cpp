#include "LMainScene.h"
#include "LGlobal.h"
#include "LInput.h"
#include "LWrite.h"

bool LMainScene::Init()
{
	return true;
}

void LMainScene::Process()
{
    std::wstring textState = L"Main Scene";
    LWrite::GetInstance().AddText(textState, 320.0f, 500.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
    std::wstring debugText = L"Key State: " + std::to_wstring(LInput::GetInstance().GetKey(DIK_1));
    LWrite::GetInstance().AddText(debugText, 320.0f, 400.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

     if (LInput::GetInstance().GetKey(DIK_1) ==  KEY_PUSH)
   
    {
        Release();
        m_pOwner->SetTransition(Event::GOUISCENE);
        return;
        
    }
     if (LInput::GetInstance().GetKey(DIK_1) == KEY_UP)
     {
         printf("asd");
     }
   /* if (LInput::GetInstance().m_KeyState['2'] == (KEY_PUSH))
    {
        Release();
        m_pOwner->SetTransition(Event::GOMAPSCENE);
        return;
    }

    if (LInput::GetInstance().m_KeyState['3'] == DWORD(KEY_PUSH))
    {
        Release();
        m_pOwner->SetTransition(Event::GOCHARACTERSCENE);
        return;
    }

    if (LInput::GetInstance().m_KeyState['4'] == DWORD(KEY_PUSH))
    {
        Release();
        m_pOwner->SetTransition(Event::GOEFFECTSCENE);
        return;
    }*/
}

void LMainScene::Release()
{
 
}

LMainScene::LMainScene(LScene* parent) : SceneState(parent)
{
  
}
LMainScene::~LMainScene() {}