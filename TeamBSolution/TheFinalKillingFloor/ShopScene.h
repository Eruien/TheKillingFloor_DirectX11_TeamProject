#pragma once
#include "LScene.h"

class ShopScene : public SceneState
{
public:
	bool Init() override;
	void Render() override;
	void Process() override;
	void Release();
public:
	ShopScene(LScene* parent);
	virtual ~ShopScene();
	

};

