#pragma once
#include "LObject.h"

class LPlaneShape : public LObject
{
public:
	bool CreateVertexData() override;
	bool CreateIndexData() override;
	bool SetScreenVertex(float x,
		float y,
		float w,
		float h,
		TVector2 vScreen);
public:
	LPlaneShape();
	virtual ~LPlaneShape();
};

