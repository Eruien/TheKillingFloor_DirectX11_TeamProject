#include "PickingUI.h"
#include "KObject.h"
#include "LGlobal.h"
#include "LInput.h"
#include "LWrite.h"
#include "TMath.h"
#include "LStd.h"
#include "Imgui_ObjectDetail.h"
#include "UIManager.h"

PickingUI::PickingUI() : MonoBehaviour(L"PickingUI")
{
	
}
PickingUI::~PickingUI()
{
}
void PickingUI::Init()
{
}

void PickingUI::Frame()
{
	//마우스, 오브젝트 좌표 출력
	

	/*LWrite::GetInstance().AddText(to_wstring(GetGameObject()->m_vPosition.x), 100, 300);
	LWrite::GetInstance().AddText(to_wstring(LINPUT.GetInstance().GetMousePos().x), 200, 400);
	LWrite::GetInstance().AddText(to_wstring(LGlobal::g_pUICamera->m_vCameraPos.z), 300, 500);*/
	//LWrite::GetInstance().AddText(to_wstring(GetGameObject()->GetRect().x), 100, 300);
	//LWrite::GetInstance().AddText(to_wstring(GetGameObject()->GetRect().y), 200, 300);
	//LWrite::GetInstance().AddText(to_wstring(GetGameObject()->m_vScale.x), 100, 400);
	//LWrite::GetInstance().AddText(to_wstring(GetGameObject()->m_vScale.y), 100, 450);

	POINT mouse ;
	mouse.x = LINPUT.GetMousePos().x - LGlobal::g_WindowWidth/2;
	mouse.y = -(LINPUT.GetMousePos().y - LGlobal::g_WindowHeight / 2);
	/*LWrite::GetInstance().AddText(to_wstring(mouse.x), 100, 400);
	LWrite::GetInstance().AddText(to_wstring(mouse.y), 300, 400);*/
	

	TRectangle rect;
	
	rect.x = GetGameObject()->GetRect().x;
	rect.y = GetGameObject()->GetRect().y;
	rect.height = GetGameObject()->GetRect().height;
	rect.width = GetGameObject()->GetRect().width;


	

	_buttonState = PICKING_STATE::NONE;// 맨마지막에
	if (TBASIS_EX::TRectangle::RectToPoint(rect, mouse)&& !Imgui_ObjectDetail::_isDialogWindow && !UIManager::s_isMouseInImGuiWindow)
	{
		if (LINPUT.m_MouseState[0] == KEY_PUSH)
		{
			_buttonState = PICKING_STATE::PRESS;
			_isSelected = true;
			if (UIManager::s_selectedObject != nullptr && GetGameObject() != UIManager::s_selectedObject)
			{
				if(!UIManager::s_selectedObject)
				(UIManager::s_selectedObject->GetScript<PickingUI>(L"PickingUI"))->SetIsSelected(false);
			}
			UIManager::s_selectedObject = GetGameObject();
		}
		else if(LINPUT.m_MouseState[0] == KEY_HOLD)
			_buttonState = PICKING_STATE::HOLD;
		else if (LINPUT.m_MouseState[0] == KEY_UP)
			_buttonState = PICKING_STATE::UP;
		else
		_buttonState = PICKING_STATE::HOVER;
	}



}
