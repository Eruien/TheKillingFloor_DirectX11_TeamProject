#pragma once
#include "Boss.h"

class BossSwiping : public NPCState
{
public:
	bool Montage(int startFrame, int endFrame);
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	bool m_Timer = false;
public:
	BossSwiping(Boss* parent);
	virtual ~BossSwiping();
};


