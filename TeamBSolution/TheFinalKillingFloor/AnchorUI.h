#pragma once
#include "MonoBehaviour.h"
class AnchorUI : public MonoBehaviour
{
public:
	AnchorUI();
	~AnchorUI();

	void Init() override;
	void Frame() override;

	void SetElementAnchor();

protected:
	float offsetX = 0;
	float offsetY = 0;
};

