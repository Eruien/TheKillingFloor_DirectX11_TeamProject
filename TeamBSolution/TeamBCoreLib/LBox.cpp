#include "LBox.h"

void LBox::CreateOBBBox(float fExtentX, float fExtentY, float fExtentZ,
	TVector3 vCenter,
	TVector3 vDirX,
	TVector3 vDirY,
	TVector3 vDirZ)
{
	m_Box.fExtent[0] = fExtentX;
	m_Box.fExtent[1] = fExtentY;
	m_Box.fExtent[2] = fExtentZ;
	m_Box.vCenter = vCenter;

	m_Box.vAxis[0] = m_Box.fExtent[0] * vDirX;
	m_Box.vAxis[1] = m_Box.fExtent[1] * vDirY;
	m_Box.vAxis[2] = m_Box.fExtent[2] * vDirZ;

	m_Box.vPos[0] = vCenter - m_Box.vAxis[0] - m_Box.vAxis[1] - m_Box.vAxis[2];
	m_Box.vPos[1] = vCenter - m_Box.vAxis[0] + m_Box.vAxis[1] - m_Box.vAxis[2];
	m_Box.vPos[2] = vCenter + m_Box.vAxis[0] + m_Box.vAxis[1] - m_Box.vAxis[2];
	m_Box.vPos[3] = vCenter + m_Box.vAxis[0] - m_Box.vAxis[1] - m_Box.vAxis[2];
	m_Box.vPos[4] = vCenter - m_Box.vAxis[0] - m_Box.vAxis[1] + m_Box.vAxis[2];
	m_Box.vPos[5] = vCenter - m_Box.vAxis[0] + m_Box.vAxis[1] + m_Box.vAxis[2];
	m_Box.vPos[6] = vCenter + m_Box.vAxis[0] + m_Box.vAxis[1] + m_Box.vAxis[2];
	m_Box.vPos[7] = vCenter + m_Box.vAxis[0] - m_Box.vAxis[1] + m_Box.vAxis[2];

	m_Box.vMax = m_Box.vPos[0];
	m_Box.vMin = m_Box.vPos[0];

	for (int iPoint = 1; iPoint < 8; iPoint++)
	{
		if (m_Box.vMax.x < m_Box.vPos[iPoint].x)
		{
			m_Box.vMax.x = m_Box.vPos[iPoint].x;
		}
		if (m_Box.vMax.y < m_Box.vPos[iPoint].y)
		{
			m_Box.vMax.y = m_Box.vPos[iPoint].y;
		}
		if (m_Box.vMax.z < m_Box.vPos[iPoint].z)
		{
			m_Box.vMax.z = m_Box.vPos[iPoint].z;
		}

		if (m_Box.vMin.x > m_Box.vPos[iPoint].x)
		{
			m_Box.vMin.x = m_Box.vPos[iPoint].x;
		}
		if (m_Box.vMin.y > m_Box.vPos[iPoint].y)
		{
			m_Box.vMin.y = m_Box.vPos[iPoint].y;
		}
		if (m_Box.vMin.z > m_Box.vPos[iPoint].z)
		{
			m_Box.vMin.z = m_Box.vPos[iPoint].z;
		}
	}
	D3DXVec3Normalize(&m_Box.vAxis[0], &m_Box.vAxis[0]);
	D3DXVec3Normalize(&m_Box.vAxis[1], &m_Box.vAxis[1]);
	D3DXVec3Normalize(&m_Box.vAxis[2], &m_Box.vAxis[2]);

	fTall = m_Box.vMax.y - m_Box.vMin.y;
}

void LBox::UpdateOBBBoxPosition(TVector3 vCenter)
{
	m_Box.vCenter = vCenter;
}

bool LBox::CreateVertexData()
{
	m_VertexList.resize(24);
	m_VertexList[0] = SimpleVertex({ -1.0f, 1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f });
	m_VertexList[1] = SimpleVertex(TVector3(1.0f, 1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(1.0f, 0.0f));
	m_VertexList[2] = SimpleVertex(TVector3(1.0f, -1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(1.0f, 1.0f));
	m_VertexList[3] = SimpleVertex(TVector3(-1.0f, -1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(0.0f, 1.0f));
	// 뒷면
	m_VertexList[4] = SimpleVertex(TVector3(1.0f, 1.0f, 1.0f), TVector3(0.0f, 0.0f, 1.0f), TVector4(0.0f, 0.0f, 0.0f, 1.0f), TVector2(0.0f, 0.0f));
	m_VertexList[5] = SimpleVertex(TVector3(-1.0f, 1.0f, 1.0f), TVector3(0.0f, 0.0f, 1.0f), TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 0.0f));
	m_VertexList[6] = SimpleVertex(TVector3(-1.0f, -1.0f, 1.0f), TVector3(0.0f, 0.0f, 1.0f), TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 1.0f));
	m_VertexList[7] = SimpleVertex(TVector3(1.0f, -1.0f, 1.0f), TVector3(0.0f, 0.0f, 1.0f), TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(0.0f, 1.0f));

	// 오른쪽
	m_VertexList[8] = SimpleVertex(TVector3(1.0f, 1.0f, -1.0f), TVector3(1.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(0.0f, 0.0f));
	m_VertexList[9] = SimpleVertex(TVector3(1.0f, 1.0f, 1.0f), TVector3(1.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(1.0f, 0.0f));
	m_VertexList[10] = SimpleVertex(TVector3(1.0f, -1.0f, 1.0f), TVector3(1.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(1.0f, 1.0f));
	m_VertexList[11] = SimpleVertex(TVector3(1.0f, -1.0f, -1.0f), TVector3(1.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(0.0f, 1.0f));

	// 왼쪽
	m_VertexList[12] = SimpleVertex(TVector3(-1.0f, 1.0f, 1.0f), TVector3(-1.0f, 0.0f, 0.0f), TVector4(1.0f, 1.0f, 0.0f, 1.0f), TVector2(0.0f, 0.0f));
	m_VertexList[13] = SimpleVertex(TVector3(-1.0f, 1.0f, -1.0f), TVector3(-1.0f, 0.0f, 0.0f), TVector4(1.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 0.0f));
	m_VertexList[14] = SimpleVertex(TVector3(-1.0f, -1.0f, -1.0f), TVector3(-1.0f, 0.0f, 0.0f), TVector4(1.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 1.0f));
	m_VertexList[15] = SimpleVertex(TVector3(-1.0f, -1.0f, 1.0f), TVector3(-1.0f, 0.0f, 0.0f), TVector4(1.0f, 1.0f, 0.0f, 1.0f), TVector2(0.0f, 1.0f));

	// 윗면
	m_VertexList[16] = SimpleVertex(TVector3(-1.0f, 1.0f, 1.0f), TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 0.5f, 1.0f, 1.0f), TVector2(0.0f, 0.0f));
	m_VertexList[17] = SimpleVertex(TVector3(1.0f, 1.0f, 1.0f), TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 0.5f, 1.0f, 1.0f), TVector2(1.0f, 0.0f));
	m_VertexList[18] = SimpleVertex(TVector3(1.0f, 1.0f, -1.0f), TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 0.5f, 1.0f, 1.0f), TVector2(1.0f, 1.0f));
	m_VertexList[19] = SimpleVertex(TVector3(-1.0f, 1.0f, -1.0f), TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 0.5f, 1.0f, 1.0f), TVector2(0.0f, 1.0f));

	// 아랫면
	m_VertexList[20] = SimpleVertex(TVector3(-1.0f, -1.0f, -1.0f), TVector3(0.0f, -1.0f, 0.0f), TVector4(0.0f, 1.0f, 1.0f, 1.0f), TVector2(0.0f, 0.0f));
	m_VertexList[21] = SimpleVertex(TVector3(1.0f, -1.0f, -1.0f), TVector3(0.0f, -1.0f, 0.0f), TVector4(0.0f, 1.0f, 1.0f, 1.0f), TVector2(1.0f, 0.0f));
	m_VertexList[22] = SimpleVertex(TVector3(1.0f, -1.0f, 1.0f), TVector3(0.0f, -1.0f, 0.0f), TVector4(0.0f, 1.0f, 1.0f, 1.0f), TVector2(1.0f, 1.0f));
	m_VertexList[23] = SimpleVertex(TVector3(-1.0f, -1.0f, 1.0f), TVector3(0.0f, -1.0f, 0.0f), TVector4(0.0f, 1.0f, 1.0f, 1.0f), TVector2(0.0f, 1.0f));

	return true;
}

bool LBox::CreateIndexData()
{
	m_IndexList.resize(36);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0; 	m_IndexList[iIndex++] = 1; 	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 0;	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 3;
	m_IndexList[iIndex++] = 4; 	m_IndexList[iIndex++] = 5; 	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 4;	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 7;
	m_IndexList[iIndex++] = 8; 	m_IndexList[iIndex++] = 9; 	m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 8;	m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 11;
	m_IndexList[iIndex++] = 12; m_IndexList[iIndex++] = 13; m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 12;	m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 15;
	m_IndexList[iIndex++] = 16; m_IndexList[iIndex++] = 17; m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 16;	m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 19;
	m_IndexList[iIndex++] = 20; m_IndexList[iIndex++] = 21; m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 20;	m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 23;

	return true;
}

bool LBox::CollisionCheckAABB(LBox* other)
{
	// 각 축에서 겹치지 않는 경우가 있다면 충돌이 없음
	if (m_Box.vMax.x < other->m_Box.vMin.x || m_Box.vMin.x > other->m_Box.vMax.x) return false;
	if (m_Box.vMax.y < other->m_Box.vMin.y || m_Box.vMin.y > other->m_Box.vMax.y) return false;
	if (m_Box.vMax.z < other->m_Box.vMin.z || m_Box.vMin.z > other->m_Box.vMax.z) return false;

	// 모든 축에서 겹치는 경우만 충돌이 있음
	return true;
}

float LBox::DotProduct(const TVector3& vec1, const TVector3& vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

bool LBox::CollisionCheckOBB(LBox* other)
{
	// 이 박스의 각 축에 대해 투영된 두 박스가 겹치는지 확인
	for (int i = 0; i < 3; i++)
	{
		if (!OverlapOnAxis(this->m_Box, other->m_Box, this->m_Box.vAxis[i]))
			return false; // 축을 따라 투영했을 때 겹치지 않으면, 두 박스는 겹치지 않음
	}

	// 다른 박스의 각 축에 대해 투영된 두 박스가 겹치는지 확인
	for (int i = 0; i < 3; i++)
	{
		if (!OverlapOnAxis(this->m_Box, other->m_Box, other->m_Box.vAxis[i]))
			return false; // 축을 따라 투영했을 때 겹치지 않으면, 두 박스는 겹치지 않음
	}

	// 모든 축에 대해 투영된 두 박스가 겹치면, 두 박스는 겹침
	return true;
}

bool LBox::IsPointInBox(TVector3 point, float radius)
{
	TVector3 localPoint = point - this->m_Box.vCenter;

	for (int i = 0; i < 3; ++i)
	{
		float proj = DotProduct(localPoint, this->m_Box.vAxis[i]);

		if (proj > this->m_Box.fExtent[i])
			return false;
		if (proj < -this->m_Box.fExtent[i])
			return false;
	}

	return true;
}

bool LBox::IsSphereInBox(TVector3 sphereCenter, float radius)
{
	TVector3 localPoint = sphereCenter - this->m_Box.vCenter;
	float distanceSquared = 0.0f;

	for (int i = 0; i < 3; ++i) {
		float distance = DotProduct(localPoint, this->m_Box.vAxis[i]);

		if (distance < -this->m_Box.fExtent[i]) {
			float t = distance + this->m_Box.fExtent[i];
			distanceSquared += t * t;
		}
		else if (distance > this->m_Box.fExtent[i]) {
			float t = distance - this->m_Box.fExtent[i];
			distanceSquared += t * t;
		}
	}

	return distanceSquared <= radius * radius;
}


bool LBox::OverlapOnAxis(const T_BOX& box1, const T_BOX& box2, const TVector3& axis)
{
	// 두 박스를 축에 투영한 후의 반경을 구함
	float box1Project = box1.fExtent[0] * abs(DotProduct(axis, box1.vAxis[0])) +
		box1.fExtent[1] * abs(DotProduct(axis, box1.vAxis[1])) +
		box1.fExtent[2] * abs(DotProduct(axis, box1.vAxis[2]));
	float box2Project = box2.fExtent[0] * abs(DotProduct(axis, box2.vAxis[0])) +
		box2.fExtent[1] * abs(DotProduct(axis, box2.vAxis[1])) +
		box2.fExtent[2] * abs(DotProduct(axis, box2.vAxis[2]));

	// 두 박스의 중심 사이의 거리를 축에 투영
	float distance = abs(DotProduct(axis, box2.vCenter - box1.vCenter));

	// 두 박스가 축을 따라 겹치는지 확인
	return distance < box1Project + box2Project;
}


void LBox::SetMatrix(TMatrix* parent, TMatrix* matView, TMatrix* matProj)
{
	if (parent != nullptr)
	{
		m_matWorld._41 = (*parent)._41;
		m_matWorld._42 = (*parent)._42;
		m_matWorld._43 = (*parent)._43;
	}

	if (matView != nullptr)
	{
		m_matView = *matView;
	}

	if (matProj != nullptr)
	{
		m_matProj = *matProj;
	}

	m_cbData.matWorld = m_matWorld.Transpose();
	m_cbData.matView = m_matView.Transpose();
	m_cbData.matProj = m_matProj.Transpose();
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, nullptr, &m_cbData, 0, 0);
}

void LBox::WeaponSetMatrix(TMatrix* parent, TMatrix* matView, TMatrix* matProj)
{
	if (parent != nullptr)
	{
		TMatrix boxScale;
		D3DXMatrixScaling(&boxScale, m_vScale.x, m_vScale.y, m_vScale.z);
		m_matWorld = boxScale * (*parent);
	}

	if (matView != nullptr)
	{
		m_matView = *matView;
	}

	if (matProj != nullptr)
	{
		m_matProj = *matProj;
	}

	m_cbData.matWorld = m_matWorld.Transpose();
	m_cbData.matView = m_matView.Transpose();
	m_cbData.matProj = m_matProj.Transpose();
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, nullptr, &m_cbData, 0, 0);
}

