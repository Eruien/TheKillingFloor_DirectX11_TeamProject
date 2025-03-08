#pragma once
#include "LCore.h"

class Sample : public LCore
{
public:
	std::shared_ptr<LDebugCamera> m_DebugCamera = nullptr;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	virtual ~Sample();
};

