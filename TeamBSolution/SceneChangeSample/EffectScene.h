#pragma once
#include "LScene.h"

class EffectScene : public SceneState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	EffectScene(LScene* parent);
	virtual ~EffectScene();
};

