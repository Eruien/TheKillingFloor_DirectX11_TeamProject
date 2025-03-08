#pragma once
#include "LScene.h"
#include "LSoundMgr.h"

class LoadScene : public SceneState
{
public:
	bool Init() override;
	void Render() override;
	void Process() override;
	void Release();
public:
	LoadScene(LScene* parent);
	virtual ~LoadScene();
};

