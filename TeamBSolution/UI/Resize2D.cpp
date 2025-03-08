#include "Resize2D.h"
#include "KObject.h"
#include "LGlobal.h"
#include "LStd.h"
#include "LInput.h"
#include "LWrite.h"
#include "PickingUI.h"
#include "UIManager.h"
Resize2D::Resize2D() : MonoBehaviour(L"Resize2D")
{
}

Resize2D::~Resize2D()
{
}

void Resize2D::Init()
{
	//resize
	


	

	
}

void Resize2D::Frame()
{
	TRectangle	rTop = { GetGameObject()->GetRect().x,
	GetGameObject()->GetRect().y + GetGameObject()->GetRect().height/2,
	GetGameObject()->GetRect().width,
		_barSize };

	TRectangle	rBottom = { GetGameObject()->GetRect().x,
	GetGameObject()->GetRect().y - GetGameObject()->GetRect().height / 2,
	GetGameObject()->GetRect().width,
		_barSize };

	TRectangle	rLeft = { GetGameObject()->GetRect().x - GetGameObject()->GetRect().width / 2,
	GetGameObject()->GetRect().y,
	_barSize,
	GetGameObject()->GetRect().height };

	TRectangle	rRight = { GetGameObject()->GetRect().x + GetGameObject()->GetRect().width / 2,
	GetGameObject()->GetRect().y,
	_barSize,
	GetGameObject()->GetRect().height };
	barPicking = false;

	LWrite::GetInstance().AddText(to_wstring(GetGameObject()->GetRect().x), 300, 0);
	LWrite::GetInstance().AddText(to_wstring(GetGameObject()->GetRect().y), 300, 100);
	LWrite::GetInstance().AddText(to_wstring(GetGameObject()->GetRect().width), 300, 200);
	LWrite::GetInstance().AddText(to_wstring(GetGameObject()->GetRect().height), 300, 300);
	
	if ((GetGameObject()->GetScript<PickingUI>(L"PickingUI"))->GetIsSelected() && !UIManager::s_isMouseInImGuiWindow && !Imgui_ObjectDetail::_isDialogWindow)
	{

		if (TBASIS_EX::TRectangle::RectToPoint(rTop, { (long)MOUSEX,(long)MOUSEY }))
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENS));
			barPicking = true;
			if (LINPUT.m_MouseState[0] == KEY_HOLD)
			{
				GetGameObject()->m_vScale.y = (MOUSEY - GetGameObject()->m_vPosition.y) * 2;
				//GetGameObject()->m_vPosition.y = GetGameObject()->m_vScale.y / 2 + GetGameObject()->m_vPosition.y;

			}
		}
		else if (TBASIS_EX::TRectangle::RectToPoint(rBottom, { (long)MOUSEX,(long)MOUSEY }))
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENS));
			barPicking = true;
			if (LINPUT.m_MouseState[0] == KEY_HOLD)
			{
				GetGameObject()->m_vScale.y = -(MOUSEY - GetGameObject()->m_vPosition.y) * 2;
			}
		}
		else if (TBASIS_EX::TRectangle::RectToPoint(rLeft, { (long)MOUSEX,(long)MOUSEY }))
		{
			SetCursor(LoadCursor(NULL, IDC_SIZEWE));
			barPicking = true;
			if (LINPUT.m_MouseState[0] == KEY_HOLD)
			{
				GetGameObject()->m_vScale.x = -(MOUSEX - GetGameObject()->m_vPosition.x) * 2;
			}
		}
		else if (TBASIS_EX::TRectangle::RectToPoint(rRight, { (long)MOUSEX,(long)MOUSEY }))
		{
			SetCursor(LoadCursor(NULL, IDC_SIZEWE));
			barPicking = true;
			if (LINPUT.m_MouseState[0] == KEY_HOLD)
			{
				GetGameObject()->m_vScale.x = (MOUSEX - GetGameObject()->m_vPosition.x) * 2;
			}
		}
	}
	};
