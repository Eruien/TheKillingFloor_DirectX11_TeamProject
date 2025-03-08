#pragma once
#include "LScene.h"

class MapScene : public SceneState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	MapScene(LScene* parent);
	virtual ~MapScene();
};

