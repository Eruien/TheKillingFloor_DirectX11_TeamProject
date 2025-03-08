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

    // UI ����� �ʺ�� ����
    float elementWidth;
    float elementHeight;

    // UI ����� ��ǥ
    float elementX;
    float elementY;
    // ȭ���� �ʺ�� ���� ���ϱ� (���� DirectX �ڵ�δ� �̸� ��� ����� �־�� �մϴ�.)
    screenWidth = LGlobal::g_WindowWidth;
    screenHeight = LGlobal::g_WindowHeight;

    // UI ����� �ʺ�� ���� ����
    elementWidth = GetGameObject()->GetRect().width;
    elementHeight = GetGameObject()->GetRect().height;

    // UI ����� ��ǥ �����ϱ� (��� ����)
    elementX = (screenWidth - elementWidth) / 2.0f;
    elementY = (screenHeight - elementHeight) / 2.0f;
    GetGameObject()->m_vPosition.x = elementX;
    GetGameObject()->m_vPosition.y = elementY;

	//offsetX = (LGlobal::g_WindowWidth - GetGameObject()->GetRect().width) / 2.0f;
	//offsetY = (LGlobal::g_WindowHeight - GetGameObject()->GetRect().height) / 2.0f;
	//GetGameObject()->m_vPosition.x = offsetX;
	//GetGameObject()->m_vPosition.y = offsetY;
}


// DirectX 11���� UI ����� ��Ŀ�� �����ϴ� �Լ�
void AnchorUI::SetElementAnchor() {

}


