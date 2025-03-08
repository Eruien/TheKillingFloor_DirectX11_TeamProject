#include "TestPlane.h"

bool TestPlane::CreateVertexData()
{
	m_VertexList.resize(10);
	m_VertexList[0] = SimpleVertex(TVector3(-1.0f, 1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 0.0f, 0.0f, 0.0f), TVector2(0.0f, 0.0f));
	m_VertexList[1] = SimpleVertex(TVector3(-0.5f, 1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 0.0f, 0.0f, 0.0f), TVector2(0.25f, 0.0f));
	m_VertexList[2] = SimpleVertex(TVector3(0.0f, 1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 0.0f, 0.0f, 0.0f), TVector2(0.5f, 0.0f));
	m_VertexList[3] = SimpleVertex(TVector3(0.5f, 1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 0.0f, 0.0f, 0.0f), TVector2(0.75f, 0.0f));
	m_VertexList[4] = SimpleVertex(TVector3(1.0f, 1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 0.0f, 0.0f, 0.0f), TVector2(1.0f, 0.0f));

	m_VertexList[5] = SimpleVertex(TVector3(-1.0f, 0.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 0.0f, 0.0f, 0.0f), TVector2(0.0f, 1.0f));
	m_VertexList[6] = SimpleVertex(TVector3(-0.5f, 0.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 0.0f, 0.0f, 0.0f), TVector2(0.25f, 1.0f));
	m_VertexList[7] = SimpleVertex(TVector3(0.0f, 0.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 0.0f, 0.0f, 0.0f), TVector2(0.5f, 1.0f));
	m_VertexList[8] = SimpleVertex(TVector3(0.5f, 0.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 0.0f, 0.0f, 0.0f), TVector2(0.75f, 1.0f));
	m_VertexList[9] = SimpleVertex(TVector3(1.0f, 0.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(0.0f, 0.0f, 0.0f, 0.0f), TVector2(1.0f, 1.0f));

	return true;
}

bool TestPlane::CreateIndexData()
{
	m_IndexList.resize(24);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0; 	m_IndexList[iIndex++] = 1; 	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 0;	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 5;
	m_IndexList[iIndex++] = 1; 	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 7; 	m_IndexList[iIndex++] = 1;	m_IndexList[iIndex++] = 7; 	m_IndexList[iIndex++] = 6;
	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 3; 	m_IndexList[iIndex++] = 8;  m_IndexList[iIndex++] = 2;	m_IndexList[iIndex++] = 8;  m_IndexList[iIndex++] = 7;
	m_IndexList[iIndex++] = 3; 	m_IndexList[iIndex++] = 4; 	m_IndexList[iIndex++] = 9;  m_IndexList[iIndex++] = 3;	m_IndexList[iIndex++] = 9;  m_IndexList[iIndex++] = 8;
	return true;
}