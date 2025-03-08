#include "LPlayer.h"

class PlayerTakeDamage : public PlayerState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	PlayerTakeDamage(LPlayer* parent);
	virtual ~PlayerTakeDamage();
};


