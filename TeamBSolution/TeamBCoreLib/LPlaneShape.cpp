#include "LPlaneShape.h"

LPlaneShape::LPlaneShape(void)
{
}

LPlaneShape::~LPlaneShape(void)
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                            TPlaneShape
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// x,y는 시작 스크린 위치, w,h 가로 폭, 세로 폭, vScreen은 윈도우 가로 및 세로 크기
bool LPlaneShape::SetScreenVertex(float x,
	float y,
	float w,
	float h,
	TVector2 vScreen)
{
	m_VertexList.resize(4);

	TVector2 vPoint;
	// 0 ~ 1
	vPoint.x = x / vScreen.x;
	vPoint.y = y / vScreen.y;
	// 0~1 => -1 ~ +1
	vPoint.x = vPoint.x * 2.0f - 1.0f;
	vPoint.y = -(vPoint.y * 2.0f - 1.0f);

	TVector2 vOffset;
	// 0 ~ 1
	vOffset.x = (x + w) / vScreen.x;
	vOffset.y = (y + h) / vScreen.y;
	// 0~1 => -1 ~ +1
	vOffset.x = (vOffset.x * 2.0f - 1.0f) - vPoint.x;
	vOffset.y = vPoint.y + (vOffset.y * 2.0f - 1.0f);

	m_VertexList[0].p.x = vPoint.x;
	m_VertexList[0].p.y = vPoint.y;
	m_VertexList[0].p.z = 0.5f;

	m_VertexList[1].p.x = vPoint.x + vOffset.x;
	m_VertexList[1].p.y = vPoint.y;
	m_VertexList[1].p.z = 0.5f;

	m_VertexList[2].p.x = vPoint.x + vOffset.x;
	m_VertexList[2].p.y = vPoint.y - vOffset.y;
	m_VertexList[2].p.z = 0.5f;

	m_VertexList[3].p.x = vPoint.x;
	m_VertexList[3].p.y = vPoint.y - vOffset.y;
	m_VertexList[3].p.z = 0.5f;
	return true;
}

bool LPlaneShape::CreateVertexData()
{
	if (m_VertexList.size() <= 0)
	{
		m_VertexList.resize(4);
		m_VertexList[0] = SimpleVertex(TVector3(-1.0f, 1.0f, 0.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector2(0.0f, 0.0f));
		m_VertexList[1] = SimpleVertex(TVector3(1.0f, 1.0f, 0.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector2(1.0f, 0.0f));
		m_VertexList[2] = SimpleVertex(TVector3(1.0f, -1.0f, 0.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector2(1.0f, 1.0f));
		m_VertexList[3] = SimpleVertex(TVector3(-1.0f, -1.0f, 0.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector2(0.0f, 1.0f));
	}
	else
	{
		m_VertexList[0] = SimpleVertex(m_VertexList[0].p, TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector2(0.0f, 0.0f));
		m_VertexList[1] = SimpleVertex(m_VertexList[1].p, TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector2(1.0f, 0.0f));
		m_VertexList[2] = SimpleVertex(m_VertexList[2].p, TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector2(1.0f, 1.0f));
		m_VertexList[3] = SimpleVertex(m_VertexList[3].p, TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector2(0.0f, 1.0f));

	}

	return true;
}

bool LPlaneShape::CreateIndexData()
{
	m_IndexList.resize(6);
	m_IndexList[0] = 0;
	m_IndexList[1] = 1;
	m_IndexList[2] = 2;
	m_IndexList[3] = 0;
	m_IndexList[4] = 2;
	m_IndexList[5] = 3;

	return true;
}