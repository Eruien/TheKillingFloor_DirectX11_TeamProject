#include "LPlayer.h"

class PlayerRush : public PlayerState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	PlayerRush(LPlayer* parent);
	virtual ~PlayerRush();
};


