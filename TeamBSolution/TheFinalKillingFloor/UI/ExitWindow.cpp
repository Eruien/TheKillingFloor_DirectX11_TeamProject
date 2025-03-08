#include "ExitWindow.h"
#include "PickingUI.h"
#include "KObject.h"
#include "LGlobal.h"
#include "UIManager.h"
ExitWindow::ExitWindow() : MonoBehaviour(L"ExitWindow")
{
}

ExitWindow::~ExitWindow()
{
}

void ExitWindow::Frame()
{
	if (!UIManager::GetInstance()._editMode)
	{
		if (GetGameObject()->GetScript<PickingUI>(L"PickingUI")->GetButtonState() == PICKING_STATE::UP)
			DestroyWindow(LGlobal::g_hWnd);
	}
}
