#pragma once
#include "LNPC.h"

class KObject;

class Tank : public LNPC
{
public:
	State GetState() override;
	void FSM(FSMType fsmType) override;
	void SetTransition(Event inputEvent) override;
	void Move(TVector3 target) override;
	void RushMove() override;
	void ComboMove() override;
	int GetRandomNumber() override;
public:
	virtual	void Process() override;
	bool Frame() override;
	bool Render() override;
	bool RenderMark() override;
public:
	Tank() {};
	Tank(LPlayer* player);
};

