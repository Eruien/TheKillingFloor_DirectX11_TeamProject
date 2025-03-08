#pragma once
#include "LCore.h"
#include "LScene.h"

class Sample : public LCore
{
public:
	std::shared_ptr<LDebugCamera> m_DebugCamera = nullptr;
	LScene* m_Scene = nullptr;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	virtual ~Sample();
};

