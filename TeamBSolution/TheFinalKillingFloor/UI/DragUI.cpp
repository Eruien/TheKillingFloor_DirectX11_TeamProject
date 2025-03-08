#include "DragUI.h"
#include "PickingUI.h"
#include "KObject.h"
#include "LStd.h"
#include "LInput.h"
#include "LGlobal.h"
#include "LWrite.h"
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
    //LWrite::GetInstance().AddText(to_wstring(offset.x), 200, 300, { 1,1,1,1 });
    //LWrite::GetInstance().AddText(to_wstring(offset.y), 200, 400, { 1,1,1,1 });

    //LWrite::GetInstance().AddText(to_wstring((TVector3((float)MOUSEsX, (float)MOUSEsY, 1) + offset).x), 200, 500,{1,1,1,1});
    //LWrite::GetInstance().AddText(to_wstring((TVector3((float)MOUSEsX, (float)MOUSEsY, 1) + offset).x), 200, 600, { 1,1,1,1 });

   // LWrite::GetInstance().AddText(to_wstring(MOUSEsX), 500, 300, { 1,1,1,1 });
   // LWrite::GetInstance().AddText(to_wstring(MOUSEsY), 500, 400, { 1,1,1,1 });

    
	if ( !UIManager::s_isMouseInImGuiWindow && UIManager::s_selectedObject == GetGameObject())
	{


        if ((GetGameObject()->GetScript<PickingUI>(L"PickingUI"))->GetButtonState() == PICKING_STATE::HOLD && _resize2D!=nullptr)
        {
            if (!_resize2D->GetisBarPicking())
            {
                if (!_isDragging) {
                    // ���콺 ��ư�� ó�� ������ ��
                    offset = GetGameObject()->m_vPosition - TVector3((float)MOUSEsX, (float)MOUSEsY, 1);
                    _isDragging = true;

                }
                else {
                    // ���콺 ��ư�� ���� ���¿��� ���콺�� �̵��ϴ� ���
                    GetGameObject()->SetPos(TVector3((float)MOUSEsX, (float)MOUSEsY, 1) + offset);
                }
            }
        }
       else if ((GetGameObject()->GetScript<PickingUI>(L"PickingUI"))->GetButtonState() == PICKING_STATE::HOLD && _resize2D == nullptr)
       {
           if (!_isDragging) {
               // ���콺 ��ư�� ó�� ������ ��
               offset = GetGameObject()->m_vPosition - TVector3((float)MOUSEsX, (float)MOUSEsY, 1);
               _isDragging = true;
           }
           else {
               // ���콺 ��ư�� ���� ���¿��� ���콺�� �̵��ϴ� ���
               GetGameObject()->SetPos(TVector3((float)MOUSEsX, (float)MOUSEsY, 1) + offset);
           }
       }
        else {
            // ���콺 ��ư�� ������ ���
            _isDragging = false;
        }

	}
}
