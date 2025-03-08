#pragma once
#include "LScene.h"

class EndScene : public SceneState
{
public:
	bool Init() override;
	void Render() override;
	void Process() override;
	void Release();
public:
	EndScene(LScene* parent);
	virtual ~EndScene();
};

