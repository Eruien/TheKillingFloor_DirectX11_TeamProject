#include "DragUI.h"
#include "PickingUI.h"
#include "KObject.h"
#include "LStd.h"
#include "LInput.h"
#include "LGlobal.h"
#include "Resize2D.h"
#include "UIManager.h"
DragUI::DragUI() : MonoBehaviour(L"DragUI")
{
}

DragUI::~DragUI()
{
}

void DragUI::Init()
{
	if ((GetGameObject()->GetScript<Resize2D>(L"Resize2D")) != nullptr)
	{
		_resize2D =(GetGameObject()->GetScript<Resize2D>(L"Resize2D"));
	}
}

void DragUI::Frame()
{
	if ((GetGameObject()->GetScript<PickingUI>(L"PickingUI"))->GetIsSelected() && !UIManager::s_isMouseInImGuiWindow )
	{


        if ((GetGameObject()->GetScript<PickingUI>(L"PickingUI"))->GetButtonState() == PICKING_STATE::HOLD && _resize2D!=nullptr) {
            if (!_resize2D->GetisBarPicking())
            {
                if (!_isDragging) {
                    // ���콺 ��ư�� ó�� ������ ��
                    offset = GetGameObject()->m_vPosition - TVector3((float)MOUSEX, (float)MOUSEY, 1);
                    _isDragging = true;
                }
                else {
                    // ���콺 ��ư�� ���� ���¿��� ���콺�� �̵��ϴ� ���
                    GetGameObject()->SetPos(TVector3((float)MOUSEX, (float)MOUSEY, 1) + offset);
                }
            }
        }
        else if ((GetGameObject()->GetScript<PickingUI>(L"PickingUI"))->GetButtonState() == PICKING_STATE::HOLD && _resize2D == nullptr)
        {
            if (!_isDragging) {
                // ���콺 ��ư�� ó�� ������ ��
                offset = GetGameObject()->m_vPosition - TVector3((float)MOUSEX, (float)MOUSEY, 1);
                _isDragging = true;
            }
            else {
                // ���콺 ��ư�� ���� ���¿��� ���콺�� �̵��ϴ� ���
                GetGameObject()->SetPos(TVector3((float)MOUSEX, (float)MOUSEY, 1) + offset);
            }
        }
        else {
            // ���콺 ��ư�� ������ ���
            _isDragging = false;
        }

	}
}
