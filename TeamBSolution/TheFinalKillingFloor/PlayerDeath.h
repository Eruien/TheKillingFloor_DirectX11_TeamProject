#include "LPlayer.h"
#include "LSoundMgr.h"

class PlayerDeath : public PlayerState
{
public:
	LSound* m_DeathSound = nullptr;
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	PlayerDeath(LPlayer* parent);
	virtual ~PlayerDeath();
};
