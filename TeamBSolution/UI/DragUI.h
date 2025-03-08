#pragma once
#include "MonoBehaviour.h"
//��ũ��Ʈ �߰� �� PickingUI ��ũ��Ʈ �߰� �Ұ�
class Resize2D;
class DragUI : public MonoBehaviour
{
public:
    DragUI();
    virtual ~DragUI();
public:
    virtual void Init() override;
    virtual void Frame() override;
private:
    shared_ptr<Resize2D> _resize2D;
    bool _isDragging = false;
    TVector3 offset = { 0,0,0 };
};

