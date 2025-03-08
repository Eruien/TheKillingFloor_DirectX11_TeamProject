#pragma once
#include "LNPC.h"

class KObject;

class Zombie : public LNPC
{
public:
	bool IsAlreadyDamaged = false;

	void FSM(FSMType fsmType) override;
	void SetTransition(Event inputEvent) override;
	State GetState() override;
	void Move(TVector3 target) override;
	void MoveInstancing(TVector3 target) override;
	int GetRandomNumber() override;
public:
	virtual	void Process() override;
	bool Frame() override;
	bool FrameInstancing() override;
	bool Render() override;
	bool RenderMark() override;
public:
	Zombie() {};
	Zombie(LPlayer* player);
};
