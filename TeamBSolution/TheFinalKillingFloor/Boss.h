#pragma once
#include "LNPC.h"

class Boss;

class Boss : public LNPC
{
public:
	State GetState() override;
	void FSM(FSMType fsmType) override;
	void SetTransition(Event inputEvent) override;
	void Move(TVector3 target) override;
	void JumpAttackMove(TVector3 target) override;
	int GetRandomNumber() override;
public:
	virtual	void Process() override;
	bool Frame() override;
	bool Render() override;
	bool RenderMark() override;
public:
	Boss() {};
	Boss(LPlayer* player);
};

