#pragma once
#include "MonoBehaviour.h"
//스크립트 추가 전 PickingUI 스크립트 추가 할것
class Resize2D : public MonoBehaviour
{
public:
	Resize2D();
	virtual ~Resize2D();
public:
	virtual void Init() override;
	virtual void Frame() override;
	bool GetisBarPicking() { return barPicking; };
private:
	int _barSize = 15;
	bool barPicking = false;
};

