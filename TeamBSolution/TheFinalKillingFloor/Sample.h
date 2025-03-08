#pragma once
#include "LCore.h"
#include "LScene.h"
#include "UICamera.h"
#include "UIManager.h"

class Sample : public LCore
{
public:
	//LScene* m_Scene = nullptr;
	std::shared_ptr<UICamera> m_UICamera = nullptr;
	shared_ptr<Sample> _win;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	virtual ~Sample();
};

