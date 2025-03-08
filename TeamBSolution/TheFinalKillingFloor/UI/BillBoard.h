#pragma once
#include "MonoBehaviour.h"
class BillBoard : public MonoBehaviour
{
public:
	BillBoard();
	~BillBoard();

	void Init() override;
	void Frame() override;
};

