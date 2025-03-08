#include "AnchorUI.h"
#include "KObject.h"
#include "LGlobal.h"
AnchorUI::AnchorUI() : MonoBehaviour(L"AnchorUI")
{
}

void AnchorUI::Init()
{
}

void AnchorUI::Frame()
{
    float screenWidth;
    float screenHeight;

    // UI 요소의 너비와 높이
    float elementWidth;
    float elementHeight;

    // UI 요소의 좌표
    float elementX;
    float elementY;
    // 화면의 너비와 높이 구하기 (실제 DirectX 코드로는 이를 얻는 방법이 있어야 합니다.)
    screenWidth = LGlobal::g_WindowWidth;
    screenHeight = LGlobal::g_WindowHeight;

    // UI 요소의 너비와 높이 설정
    elementWidth = GetGameObject()->GetRect().width;
    elementHeight = GetGameObject()->GetRect().height;

    // UI 요소의 좌표 설정하기 (가운데 정렬)
    elementX = (screenWidth - elementWidth) / 2.0f;
    elementY = (screenHeight - elementHeight) / 2.0f;
    GetGameObject()->m_vPosition.x = elementX;
    GetGameObject()->m_vPosition.y = elementY;

	//offsetX = (LGlobal::g_WindowWidth - GetGameObject()->GetRect().width) / 2.0f;
	//offsetY = (LGlobal::g_WindowHeight - GetGameObject()->GetRect().height) / 2.0f;
	//GetGameObject()->m_vPosition.x = offsetX;
	//GetGameObject()->m_vPosition.y = offsetY;
}


// DirectX 11에서 UI 요소의 앵커를 설정하는 함수
void AnchorUI::SetElementAnchor() {

}


