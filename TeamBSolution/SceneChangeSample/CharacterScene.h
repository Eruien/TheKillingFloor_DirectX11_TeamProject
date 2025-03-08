#pragma once
#include "LScene.h"

class CharacterScene : public SceneState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	CharacterScene(LScene* parent);
	virtual ~CharacterScene();
};

