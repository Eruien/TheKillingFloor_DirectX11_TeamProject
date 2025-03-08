#pragma once
#include "LScene.h"

class UIScene : public SceneState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	UIScene(LScene* parent);
	virtual ~UIScene();
};

