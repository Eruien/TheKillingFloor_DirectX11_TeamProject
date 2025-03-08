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
	GetGameObject()->GetRect().y + GetGameObject()->GetRect().height /2,
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

	//LWrite::GetInstance().AddText(to_wstring(rTop.x), 300, 5);
	//LWrite::GetInstance().AddText(to_wstring(rTop.y), 300, 100);
	//LWrite::GetInstance().AddText(to_wstring(rTop.width), 300, 150);
	//LWrite::GetInstance().AddText(to_wstring(rTop.height), 300, 200);



	if (UIManager::s_selectedObject == GetGameObject() && !UIManager::s_isMouseInImGuiWindow && !Imgui_ObjectDetail::_isDialogWindow)
	{

		if (TBASIS_EX::TRectangle::RectToPoint(rTop, { (long)MOUSEsX,(long)MOUSEsY }))
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENS));
			barPicking = true;
			if (LINPUT.m_MouseState[0] == KEY_HOLD)
			{
				GetGameObject()->m_vScale.y = (MOUSEsY - GetGameObject()->m_vPosition.y) * 2;
				//GetGameObject()->m_vPosition.y = GetGameObject()->m_vScale.y / 2 + GetGameObject()->m_vPosition.y;

			}
		}
		else if (TBASIS_EX::TRectangle::RectToPoint(rBottom, { (long)MOUSEsX,(long)MOUSEsY }))
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENS));
			barPicking = true;
			if (LINPUT.m_MouseState[0] == KEY_HOLD)
			{
				GetGameObject()->m_vScale.y = -(MOUSEsY - GetGameObject()->m_vPosition.y) * 2;
			}
		}
		else if (TBASIS_EX::TRectangle::RectToPoint(rLeft, { (long)MOUSEsX,(long)MOUSEsY }))
		{
			SetCursor(LoadCursor(NULL, IDC_SIZEWE));
			barPicking = true;
			if (LINPUT.m_MouseState[0] == KEY_HOLD)
			{
				GetGameObject()->m_vScale.x = -(MOUSEsX - GetGameObject()->m_vPosition.x) * 2;
			}
		}
		else if (TBASIS_EX::TRectangle::RectToPoint(rRight, { (long)MOUSEsX,(long)MOUSEsY }))
		{
			SetCursor(LoadCursor(NULL, IDC_SIZEWE));
			barPicking = true;
			if (LINPUT.m_MouseState[0] == KEY_HOLD)
			{
				GetGameObject()->m_vScale.x = (MOUSEsX - GetGameObject()->m_vPosition.x) * 2;
			}
		}
	}
	};
