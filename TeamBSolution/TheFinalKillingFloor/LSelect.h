#pragma once
#include "LStd.h"
#include "LObject.h"
#include <algorithm>

struct L_RAY
{
	float fExtent;
	TVector3 vOrigin;
	TVector3 vDirection;

	L_RAY()
	{
		// fExtent > 0 : 세그먼트로 사용
		fExtent = -1;
	}
};

class LSelect
{
public:
	L_RAY m_Ray;
	TVector3 m_vIntersection;
	TMatrix m_matWorld;
	TMatrix m_matView;
	TMatrix m_matProj;
	TVector3 m_vDxR;
public:
	void SetMatrix(TMatrix* pWorld, TMatrix* pView, TMatrix* pProj);
	void Update();
	void UpdateCursorCenter();
	TVector3 GetIntersection();
	bool GetIntersection(TVector3 vStart, TVector3 vEnd, TVector3 vNormal, TVector3 v0, TVector3 v1, TVector3 v2, float* fPercentage = nullptr);
	bool GetIntersection(float fRayRange, TVector3 vNormal, TVector3 v0, TVector3 v1, TVector3 v2, float* fPercentage = nullptr);
	bool PointInPolygon(TVector3 iVertex, TVector3 faceNormal, TVector3 v0, TVector3 v1, TVector3 v2);
	bool ObjectIsIntersection(LObject* obj);
	bool ChkOBBToRay(T_BOX* pBox, L_RAY* pRay = nullptr);
public:
	LSelect();
	virtual ~LSelect();
};
