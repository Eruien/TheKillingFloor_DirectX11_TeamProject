#pragma once
#include "LCore.h"
#include "LMap.h"

class Sample : public LCore
{
public:
	std::shared_ptr<LDebugCamera> m_DebugCamera = nullptr;
	LMap* m_CustomMap = nullptr;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	virtual ~Sample();
};

