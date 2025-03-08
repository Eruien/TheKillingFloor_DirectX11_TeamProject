#include "LPlayer.h"

class PlayerReload : public PlayerState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	PlayerReload(LPlayer* parent);
	virtual ~PlayerReload();
};

