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
                    // 마우스 버튼이 처음 눌렸을 때
                    offset = GetGameObject()->m_vPosition - TVector3((float)MOUSEX, (float)MOUSEY, 1);
                    _isDragging = true;
                }
                else {
                    // 마우스 버튼이 눌린 상태에서 마우스가 이동하는 경우
                    GetGameObject()->SetPos(TVector3((float)MOUSEX, (float)MOUSEY, 1) + offset);
                }
            }
        }
        else if ((GetGameObject()->GetScript<PickingUI>(L"PickingUI"))->GetButtonState() == PICKING_STATE::HOLD && _resize2D == nullptr)
        {
            if (!_isDragging) {
                // 마우스 버튼이 처음 눌렸을 때
                offset = GetGameObject()->m_vPosition - TVector3((float)MOUSEX, (float)MOUSEY, 1);
                _isDragging = true;
            }
            else {
                // 마우스 버튼이 눌린 상태에서 마우스가 이동하는 경우
                GetGameObject()->SetPos(TVector3((float)MOUSEX, (float)MOUSEY, 1) + offset);
            }
        }
        else {
            // 마우스 버튼이 떼어진 경우
            _isDragging = false;
        }

	}
}
