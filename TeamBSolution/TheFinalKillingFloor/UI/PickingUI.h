#pragma once
#include "MonoBehaviour.h"
enum class PICKING_STATE
{
	NONE,
	HOVER,
	PRESS,
	HOLD,
	UP
};
class PickingUI : public MonoBehaviour
{
public:
	PickingUI();
	~PickingUI();
	virtual void Init() override;
	virtual void Frame() override;
	PICKING_STATE GetButtonState() { return _buttonState; };
	void SetIsSelected(bool isSelected) { _isSelected = isSelected; }
	bool GetIsSelected() { return _isSelected; };

protected:
	PICKING_STATE _buttonState;
	bool _isSelected = false;
};

