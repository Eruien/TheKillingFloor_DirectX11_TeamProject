#pragma once
#include "LScene.h"

class SwordShopScene : public SceneState
{
public:
	bool Init() override;
	void Render() override;
	void Process() override;
	void Release();
public:
	SwordShopScene(LScene* parent);
	virtual ~SwordShopScene();
};

