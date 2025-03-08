#pragma once
#include "LObject.h"

class TestPlane : public LObject
{
public:
	bool CreateVertexData() override;
	bool CreateIndexData() override;
};

