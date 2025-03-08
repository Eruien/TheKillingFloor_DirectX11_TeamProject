#include "Tank.h"

class TankCombo : public NPCState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	bool m_Timer = false;
public:
	TankCombo(Tank* parent);
	virtual ~TankCombo();
};


