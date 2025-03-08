#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <unordered_map>
#include <string>
#include <cmath>
#include <windows.h>
#define L_PI (3.141592f)
#define L_EPSILON (0.0001f)
#define RadianToDegree(radian) ( radian * (180.0f / L_PI))
#define DegreeToRadian(radian) ( radian * (L_PI / 180.0f ))
#define randstep(fmin, fmax) (float(fmin) + (float(fmax) - float(fmin)) * rand() / RAND_MAX)

class LMatrix;

struct LVector2
{
	float x = 0.0f;
	float y = 0.0f;

	LVector2();
	LVector2(float fx, float fy);
};

struct LVector
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	float Length();

	static float GetDistance(LVector& p);

	bool operator == (LVector& p);

	bool operator != (LVector& p);

	LVector operator + (LVector& p);

	LVector operator - (LVector& p);

	LVector operator * (float f);

	LVector& operator *= (float f);

	LVector operator / (float f);

	LVector& operator /= (float f);

	float operator | (LVector& p);

	LVector operator ^ (LVector& p);

	LVector operator * (LMatrix const& m);

	LVector NormalVector();

	LVector NormalVector(LVector& p);

	float Angle(LVector& p);

	LVector();
	LVector(float fx, float fy, float fz);
};

struct LVector4
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;

	LVector4();
	LVector4(float fx, float fy, float fz, float fw);
};

class LRay
{
public:
	LVector vPos;
	LVector vDir;
public:
	float GetDistance(LVector& point);
public:
	LRay();
	LRay(LVector pos, LVector nor);
};

class LPlane
{
public:
	LVector vPos;
	LVector vNor;
	float d = 0.0f;
public:
	float GetDistance(LVector& p);
public:
	LPlane();
	LPlane(LVector pos, LVector nor);
	LPlane(LVector v0, LVector v1, LVector v2);
};

struct LFloat4x4
{
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};
};

class LMatrix : public LFloat4x4
{
public:
	void Identity();
public:
	LMatrix operator * (const LMatrix& matrix);

	LVector operator * (const LVector& v);

	void Translation(LVector& v);

	void Translation(float fx, float fy, float fz);

	void Scale(LVector& v);

	void Scale(float fx, float fy, float fz);

	void XRotate(float fRadian);

	void YRotate(float fRadian);

	void ZRotate(float fRadian);

	LMatrix Transpose();

	float operator() (int iRow, int iCol) const;

	float& operator() (int iRow, int iCol);

public:
	LMatrix();
};

class LMath
{
};
