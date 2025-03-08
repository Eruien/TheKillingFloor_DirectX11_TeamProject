#pragma once
#include "LObject.h"

class LBox : public LObject
{
public:
	T_BOX m_Box;
	TMatrix m_BoxWorld;
	float fTall;
public:
	void CreateOBBBox(float fExtX = 1.0f, float fExtY = 1.0f, float fExtZ = 1.0f,
		TVector3 vCenter = TVector3(0.0f, 0.0f, 0.0f),
		TVector3 vDirX = TVector3(1.0f, 0.0f, 0.0f),
		TVector3 vDirY = TVector3(0.0f, 1.0f, 0.0f),
		TVector3 vDirZ = TVector3(0.0f, 0.0f, 1.0f));
	void UpdateOBBBoxPosition(TVector3 vCenter);
	bool CreateVertexData() override;
	bool CreateIndexData() override;
	bool CollisionCheckAABB(LBox* other);
	bool CollisionCheckOBB(LBox* other);
	bool IsPointInBox(TVector3 point, float radius);
	bool IsSphereInBox(TVector3 sphereCenter, float radius);
	float DotProduct(const TVector3& vec1, const TVector3& vec2);
	bool OverlapOnAxis(const T_BOX& box1, const T_BOX& box2, const TVector3& axis);
	void SetMatrix(TMatrix* matWorld, TMatrix* matView, TMatrix* matProj) override;
	void WeaponSetMatrix(TMatrix* parent, TMatrix* matView, TMatrix* matProj);
};

