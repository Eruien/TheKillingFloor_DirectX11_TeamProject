#pragma once
#include "LScene.h"
#include "LSoundMgr.h"

class LMainScene : public SceneState
{
public:
	bool Init() override;
	void Render() override;
	void Process() override;
	void Release();
public:
	LMainScene(LScene* parent);
	virtual ~LMainScene();
};

