#include "LPlayer.h"

class PlayerTwoHandSlash : public PlayerState
{
public:
	bool IsClick = false;
	bool IsFirstClick = false;
	bool IsFirstSlashSound = true;
	bool IsSecondSlashSound = true;
	ComboType m_CurrentCombo = ComboType::INWARD;
public:
	bool Montage(int startFrame, int endFrame);
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	PlayerTwoHandSlash(LPlayer* parent);
	virtual ~PlayerTwoHandSlash();
};

