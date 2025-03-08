#pragma once
#include "LStd.h"
#include "LModel.h"
#include "LNPC.h"
#include "LGlobal.h"

class Bullet : public LModel
{
public:
	LNPC* target;
	TVector3 m_Forward = { 0,0,0 };
	bool bTarget = false;
	bool Frame() override;
	float lifeTime = 3.f;
	float lifeStart = 10.f;
};

