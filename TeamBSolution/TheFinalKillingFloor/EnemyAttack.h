#include "Zombie.h"

class EnemyAttack : public NPCState
{
public:
	bool Montage(int startFrame, int endFrame);
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	bool m_Timer = false;
public:
	EnemyAttack(Zombie* parent);
	virtual ~EnemyAttack();
};


