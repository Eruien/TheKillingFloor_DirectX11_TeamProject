#pragma once
#include "LMath.h"

struct LRectUV
{
	LVector m_Min;
	LVector m_Max;
};

struct LBox
{
	bool isCollision = false;
	float m_fWidth = 0.0f;
	float m_fHeight = 0.0f;
	float m_fDepth = 0.0f;
	LVector m_Point[8];
	LVector m_Center;
	LVector m_Half;
	LVector m_Min;
	LVector m_Max;

	bool InPoint(LVector& p);

	bool RectInPoint(LVector& p);

	bool InBox(LBox& box);

	bool CollisionRect(LBox& rect);

	bool CollisionBox(LBox& box);

	void Set(float fx, float fy, float fz, float fw, float fh, float fd);

	void Set(LVector p, float fw, float fh, float fd);

	void Set(float fx, float fy, float fz);

	void Set(LVector p);

	bool operator == (LBox& box);

	bool operator != (LBox& box);

	LBox operator + (LBox& box);

	LBox operator - (LBox& box);

	LBox();
	LBox(float fx, float fy, float fz, float fw, float fh, float fd);
};

struct LCircle
{
	LVector vCenter;
	float fRadius;
};

struct LSphere
{
	LVector vCenter;
	float fRadius;
};