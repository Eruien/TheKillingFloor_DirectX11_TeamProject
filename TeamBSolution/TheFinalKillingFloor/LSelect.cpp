#include "LSelect.h"
#include "LGlobal.h"
#include "LInput.h"
#include "LWrite.h"

void LSelect::SetMatrix(TMatrix* pWorld, TMatrix* pView, TMatrix* pProj)
{
	if (pWorld != nullptr)
	{
		m_matWorld = *pWorld;
	}

	if (pView != nullptr)
	{
		m_matView = *pView;
	}

	if (pProj != nullptr)
	{
		m_matProj = *pProj;
	}

	UpdateCursorCenter();
}

void LSelect::Update()
{
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(LGlobal::g_hWnd, &ptCursor);

	TVector3 v;
	v.x = (((2.0f * ptCursor.x) / LGlobal::g_WindowWidth) - 1) / m_matProj._11;
	v.y = -(((2.0f * ptCursor.y) / LGlobal::g_WindowHeight) - 1) / m_matProj._22;
	v.z = 1.0f;

	TMatrix mWorldView = m_matWorld * m_matView;
	TMatrix m;
	D3DXMatrixInverse(&m, NULL, &mWorldView);

	m_Ray.vOrigin = TVector3(0.0f, 0.0f, 0.0f);
	m_Ray.vDirection = TVector3(v.x, v.y, v.z);
	D3DXVec3TransformCoord(&m_Ray.vOrigin, &m_Ray.vOrigin, &m);
	D3DXVec3TransformNormal(&m_Ray.vDirection, &m_Ray.vDirection, &m);
	D3DXVec3Normalize(&m_Ray.vDirection, &m_Ray.vDirection);
}

void LSelect::UpdateCursorCenter()
{
	RECT clientRect;
	GetClientRect(LGlobal::g_hWnd, &clientRect);

	int clientCenterX = (clientRect.right - clientRect.left) / 2;
	int clientCenterY = (clientRect.bottom - clientRect.top) / 2;
	POINT point = { clientCenterX, clientCenterY };
	ClientToScreen(LGlobal::g_hWnd, &point);
	ScreenToClient(LGlobal::g_hWnd, &point);

	TVector3 v;
	v.x = (((2.0f * point.x) / LGlobal::g_WindowWidth) - 1) / m_matProj._11;
	v.y = -(((2.0f * point.y) / LGlobal::g_WindowHeight) - 1) / m_matProj._22;
	v.z = 1.0f;

	TMatrix mWorldView = m_matWorld * m_matView;
	TMatrix m;
	D3DXMatrixInverse(&m, NULL, &mWorldView);

	m_Ray.vOrigin = TVector3(0.0f, 0.0f, 0.0f);
	m_Ray.vDirection = TVector3(v.x, v.y, v.z);
	D3DXVec3TransformCoord(&m_Ray.vOrigin, &m_Ray.vOrigin, &m);
	D3DXVec3TransformNormal(&m_Ray.vDirection, &m_Ray.vDirection, &m);
	D3DXVec3Normalize(&m_Ray.vDirection, &m_Ray.vDirection);
}

TVector3 LSelect::GetIntersection()
{
	return m_vIntersection;
}

bool LSelect::GetIntersection(TVector3 vStart, TVector3 vEnd, TVector3 vNormal, TVector3 v0, TVector3 v1, TVector3 v2, float* fPercentage)
{
	TVector3 vDirection = vEnd - vStart;
	// 법선벡터와 반직선 내적
	// 내적하면 삼각형의 법선벡터와 얼마나 비슷한지 성분이 나온다.
	// 그림자는 법선벡터와 비슷하게 삼각형의 위를 향해있다.
	float D = D3DXVec3Dot(&vNormal, &vDirection);
	// 레이의 시작 지점과 삼각형의 한점을 내적하면
	// 법선벡터 보다 작지만 삼각형을 덮고 살짝 올라가는 그림자가진다.
	TVector3 alen = v0 - vStart;
	float a0 = D3DXVec3Dot(&vNormal, &alen);
	float fT = a0 / D;

	if (fPercentage != nullptr)
	{
		*fPercentage = fT;
	}

	if (fT < 0.0f || fT > 1.0f)
	{
		return false;
	}

	m_vIntersection = vStart + vDirection * fT;

	return true;
}

bool LSelect::GetIntersection(float fRayRange, TVector3 vNormal, TVector3 v0, TVector3 v1, TVector3 v2, float* fPercentage)
{
	TVector3 vDirection = (m_Ray.vDirection * fRayRange) - m_Ray.vOrigin;
	// 법선벡터와 반직선 내적
	// 내적하면 삼각형의 법선벡터와 얼마나 비슷한지 성분이 나온다.
	// 그림자는 법선벡터와 비슷하게 삼각형의 위를 향해있다.
	float D = D3DXVec3Dot(&vNormal, &vDirection);
	// 레이의 시작 지점과 삼각형의 한점을 내적하면
	// 법선벡터 보다 작지만 삼각형을 덮고 살짝 올라가는 그림자가진다.
	TVector3 alen = v0 - m_Ray.vOrigin;
	float a0 = D3DXVec3Dot(&vNormal, &alen);
	float fT = a0 / D;

	if (fPercentage != nullptr)
	{
		*fPercentage = fT;
	}

	if (fT < 0.0f || fT > 1.0f)
	{
		return false;
	}

	m_vIntersection = m_Ray.vOrigin + vDirection * fT;

	return true;
}

bool LSelect::PointInPolygon(TVector3 iVertex, TVector3 faceNormal, TVector3 v0, TVector3 v1, TVector3 v2)
{
	TVector3 e0, e1, iInter, vNormal;

	e0 = v2 - v1;
	e1 = v0 - v1;
	iInter = iVertex - v1;

	D3DXVec3Cross(&vNormal, &e0, &iInter);
	D3DXVec3Normalize(&vNormal, &vNormal);
	float fDot = D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f) return false;

	D3DXVec3Cross(&vNormal, &iInter, &e1);
	D3DXVec3Normalize(&vNormal, &vNormal);
	fDot = D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f) return false;

	e0 = v0 - v2;
	e1 = v1 - v2;
	iInter = iVertex - v2;

	D3DXVec3Cross(&vNormal, &e0, &iInter);
	D3DXVec3Normalize(&vNormal, &vNormal);
	fDot = D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f) return false;

	D3DXVec3Cross(&vNormal, &iInter, &e1);
	D3DXVec3Normalize(&vNormal, &vNormal);
	fDot = D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f) return false;

	return true;
}

bool LSelect::ObjectIsIntersection(LObject* obj)
{
	TVector3 v0, v1, v2;

	for (int iFace = 0; iFace < 12; iFace++)
	{
		v0 = obj->m_VertexList[obj->m_IndexList[iFace * 3 + 0]].p;
		v1 = obj->m_VertexList[obj->m_IndexList[iFace * 3 + 1]].p;
		v2 = obj->m_VertexList[obj->m_IndexList[iFace * 3 + 2]].p;

		TVector3 vNormal, vCross;
		TVector3 e0 = v1 - v0;
		TVector3 e1 = v2 - v0;
		D3DXVec3Cross(&vCross, &e0, &e1);
		D3DXVec3Normalize(&vNormal, &vCross);

		if (GetIntersection(
			100.0f,
			vNormal,
			v0, v1, v2) && LInput::GetInstance().m_MouseState[0])
		{
			if (PointInPolygon(m_vIntersection, vNormal, v0, v1, v2))
			{
				std::wstring text = std::to_wstring(m_vIntersection.x);
				text += std::to_wstring(m_vIntersection.y);
				text += std::to_wstring(m_vIntersection.z);
				LWrite::GetInstance().AddText(text, 0.0f, 100.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
				return true;
			}
		}
	}

	return false;
}

bool LSelect::ChkOBBToRay(T_BOX* pBox, L_RAY* pRay)
{
	if (pRay == NULL) pRay = &m_Ray;
	float t_min = -999999.0f;
	float t_max = 999999.0f;
	float  f[3], fa[3], s[3], sa[3];

	TVector3 vR = pRay->vOrigin - pBox->vCenter;

	for (int v = 0; v < 3; v++)
	{
		f[v] = D3DXVec3Dot(&pBox->vAxis[v], &pRay->vDirection);
		s[v] = D3DXVec3Dot(&pBox->vAxis[v], &vR);
		fa[v] = fabs(f[v]);
		sa[v] = fabs(s[v]);

		if (sa[v] > pBox->fExtent[v] && s[v] * f[v] >= 0.0f)
			return false;

		float t1 = (-s[v] - pBox->fExtent[v]) / f[v];
		float t2 = (-s[v] + pBox->fExtent[v]) / f[v];
		if (t1 > t2)
		{
			std::swap(t1, t2);
		}
		t_min = max(t_min, t1);
		t_max = min(t_max, t2);
		if (t_min > t_max)
			return false;
	}

	float  fCross[3], fRhs;
	TVector3 vDxR;
	D3DXVec3Cross(&vDxR, &pRay->vDirection, &vR);
	// D X pBox->vAxis[0]  분리축
	fCross[0] = fabs(D3DXVec3Dot(&vDxR, &pBox->vAxis[0]));
	float fAxis2 = pBox->fExtent[1] * fa[2];
	float fAxis1 = pBox->fExtent[2] * fa[1];
	fRhs = fAxis2 + fAxis1;
	if (fCross[0] > fRhs)
	{
		m_vDxR = vDxR;
		return false;
	}
	// D x pBox->vAxis[1]  분리축
	fCross[1] = fabs(D3DXVec3Dot(&vDxR, &pBox->vAxis[1]));
	fRhs = pBox->fExtent[0] * fa[2] + pBox->fExtent[2] * fa[0];
	if (fCross[1] > fRhs)
	{
		m_vDxR = vDxR;
		return false;
	}
	// D x pBox->vAxis[2]  분리축
	fCross[2] = fabs(D3DXVec3Dot(&vDxR, &pBox->vAxis[2]));
	fRhs = pBox->fExtent[0] * fa[1] + pBox->fExtent[1] * fa[0];
	if (fCross[2] > fRhs)
	{
		m_vDxR = vDxR;
		return false;
	}

	m_vIntersection = pRay->vOrigin + pRay->vDirection * t_min;
	
	return true;
}

LSelect::LSelect()
{
	m_vIntersection = TVector3(0, 0, 0);

	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}

LSelect::~LSelect()
{

}