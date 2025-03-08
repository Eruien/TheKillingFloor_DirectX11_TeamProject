#include "LMath.h"

// LVector

LVector2::LVector2() {}
LVector2::LVector2(float fx, float fy) : x(fx), y(fy) {}

float LVector::Length()
{
	return sqrt(x * x + y * y + z * z);
}

float LVector::GetDistance(LVector& p)
{
	return sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
}

bool LVector::operator == (LVector& p)
{
	if (fabs(x - p.x) < L_EPSILON)
	{
		if (fabs(y - p.y) < L_EPSILON)
		{
			if (fabs(z - p.z) < L_EPSILON)
			{
				return true;
			}
		}
	}

	return false;
}

bool LVector::operator != (LVector& p)
{
	return (*this == p);
}

LVector LVector::operator + (LVector& p)
{
	return LVector(x + p.x, y + p.y, z + p.z);
}

LVector LVector::operator - (LVector& p)
{
	return LVector(x - p.x, y - p.y, z - p.z);
}

LVector LVector::operator * (float f)
{
	return LVector(x * f, y * f, z * f);
}

LVector& LVector::operator *= (float f)
{
	x = x * f;
	y = y * f;
	z = z * f;
	return *this;
}

LVector LVector::operator / (float f)
{
	return LVector(x / f, y / f, z / f);
}

LVector& LVector::operator /= (float f)
{
	x = x / f;
	y = y / f;
	z = z / f;
	return *this;
}

float LVector::operator | (LVector& p)
{
	return x * p.x + y * p.y + z * p.z;
}

LVector LVector::operator ^ (LVector& p)
{
	return LVector(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
}

LVector LVector::operator * (LMatrix const& m)
{
	LVector4 t;
	t.x = x * m._11 + y * m._21 + z * m._31 + 1.0f * m._41;
	t.y = x * m._12 + y * m._22 + z * m._32 + 1.0f * m._42;
	t.z = x * m._13 + y * m._23 + z * m._33 + 1.0f * m._43;
	t.w = x * m._14 + y * m._24 + z * m._34 + 1.0f * m._44;

	return LVector(t.x, t.y, t.z);
}

LVector LVector::NormalVector()
{
	return  *this / Length();
}

LVector LVector::NormalVector(LVector& p)
{
	return  p / p.Length();
}

float LVector::Angle(LVector& p)
{
	LVector normalVector1 = NormalVector();
	LVector normalVector2 = p.NormalVector();
	float dotProduct = normalVector1 | normalVector2;
	float radian = acos(dotProduct);
	float degree = RadianToDegree(radian);
	return degree;
}

LVector::LVector() {}
LVector::LVector(float fx, float fy, float fz) : x(fx), y(fy), z(fz) {}

LVector4::LVector4() {}
LVector4::LVector4(float fx, float fy, float fz, float fw) : x(fx), y(fy), z(fz), w(fw) {}

//LRay
float LRay::GetDistance(LVector& point)
{
	LVector pVector = point - vPos;
	float k = (pVector | vDir) / (vDir | vDir);
	LVector p = vDir * k;
	LVector w = p - pVector;
	return w.Length();
}

LRay::LRay() {}
LRay::LRay(LVector pos, LVector nor) : vPos(pos), vDir(nor) {}

//LPlane
float LPlane::GetDistance(LVector& p)
{
	return p.x * vNor.x + p.y * vNor.y + p.z * vNor.z;
}

LPlane::LPlane() {}
LPlane::LPlane(LVector pos, LVector nor) : vPos(pos), vNor(nor) {}
LPlane::LPlane(LVector v0, LVector v1, LVector v2)
{
	vPos = v0;
	LVector vector1 = v1 - v0;
	LVector vector2 = v2 - v0;
	vNor = (vector1 ^ vector2).NormalVector();
}

//LMatrix
void LMatrix::Identity()
{
	_11 = _12 = _13 = _14 = 0.0f;
	_21 = _22 = _23 = _24 = 0.0f;
	_31 = _32 = _33 = _34 = 0.0f;
	_41 = _42 = _43 = _44 = 0.0f;
	_11 = _22 = _33 = _44 = 1.0f;
}

LMatrix LMatrix::operator * (const LMatrix& matrix)
{
	LMatrix ret;
	for (int iRow = 0; iRow < 4; iRow++)
	{
		for (int iCol = 0; iCol < 4; iCol++)
		{
			ret.m[iRow][iCol] =
				m[iRow][0] * matrix.m[0][iCol] +
				m[iRow][1] * matrix.m[1][iCol] +
				m[iRow][2] * matrix.m[2][iCol] +
				m[iRow][3] * matrix.m[3][iCol];
		}
	}

	return ret;
}

LVector LMatrix::operator * (const LVector& v)
{
	LVector ret;
	ret.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + 1.0f * m[3][0];
	ret.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + 1.0f * m[3][1];
	ret.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + 1.0f * m[3][2];

	return ret;
}

void LMatrix::Translation(LVector& v)
{
	_41 = v.x;
	_42 = v.y;
	_43 = v.z;
}

void LMatrix::Translation(float fx, float fy, float fz)
{
	_41 = fx;
	_42 = fy;
	_43 = fz;
}

void LMatrix::Scale(LVector& v)
{
	_11 = v.x;
	_22 = v.y;
	_33 = v.z;
}

void LMatrix::Scale(float fx, float fy, float fz)
{
	_11 = fx;
	_22 = fy;
	_33 = fz;
}

void LMatrix::XRotate(float fRadian)
{
	float cosine = cos(fRadian);
	float sine = sin(fRadian);
	_22 = cosine; _23 = sine;
	_32 = -sine; _33 = cosine;
}

void LMatrix::YRotate(float fRadian)
{
	float cosine = cos(fRadian);
	float sine = sin(fRadian);
	_11 = cosine; _13 = -sine;
	_31 = sine; _33 = cosine;
}

void LMatrix::ZRotate(float fRadian)
{
	float cosine = cos(fRadian);
	float sine = sin(fRadian);
	_11 = cosine; _12 = sine;
	_21 = -sine; _22 = cosine;
}

LMatrix LMatrix::Transpose()
{
	LMatrix ret;
	ret._11 = _11; ret._12 = _21; ret._13 = _31; ret._14 = _41;
	ret._21 = _12; ret._22 = _22; ret._23 = _32; ret._24 = _42;
	ret._31 = _13; ret._32 = _23; ret._33 = _33; ret._34 = _43;
	ret._41 = _14; ret._42 = _24; ret._43 = _34; ret._44 = _44;

	return ret;
}

float LMatrix::operator() (int iRow, int iCol) const
{
	return m[iRow][iCol];
}

float& LMatrix::operator() (int iRow, int iCol)
{
	return m[iRow][iCol];
}

LMatrix::LMatrix()
{
	Identity();
}
