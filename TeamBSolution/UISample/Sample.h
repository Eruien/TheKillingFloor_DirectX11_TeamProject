#pragma once
#include "LCore.h"
#include "LSpriteUVObj.h"

class Sample : public LCore
{
public:
	std::shared_ptr<LDebugCamera> m_DebugCamera = nullptr;
	LSpriteUVObj* razer = nullptr;
public:
	void SpriteUV();
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	virtual ~Sample();
};