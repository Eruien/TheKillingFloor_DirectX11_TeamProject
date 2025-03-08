#pragma once
#include "LObject.h"

class LPlaneObj : public LObject
{
public:
	bool CreateVertexBuffer() override;
public:
	bool Init() override;
	bool Frame() override;
public:
	virtual ~LPlaneObj() {}
};


