#include "LCamera.h"

TMatrix LCamera::CreateLookAt(TVector3 vPosition, TVector3 vTarget, TVector3 vUp)
{
    m_vTargetPos = vTarget;
    m_vCameraPos = vPosition;

    TVector3 vDirection = vTarget - vPosition;      // 
    D3DXVec3Normalize(&vDirection, &vDirection);
    float fDot = D3DXVec3Dot(&vUp, &vDirection);
    TVector3 vC = vDirection * fDot;
    TVector3 vUpVector = vUp - vC;
    D3DXVec3Normalize(&vUpVector, &vUpVector);
    TVector3 vRightVector;
    D3DXVec3Cross(&vRightVector, &vUpVector, &vDirection);

    // 사실상 이것도 전치 한것이다 
    m_matView._11 = vRightVector.x;	m_matView._12 = vUpVector.x;		m_matView._13 = vDirection.x;
    m_matView._21 = vRightVector.y;	m_matView._22 = vUpVector.y;		m_matView._23 = vDirection.y;
    m_matView._31 = vRightVector.z;	m_matView._32 = vUpVector.z;		m_matView._33 = vDirection.z;

    m_matView._41 = -(vPosition.x * m_matView._11 + vPosition.y * m_matView._21 + vPosition.z * m_matView._31);
    m_matView._42 = -(vPosition.x * m_matView._12 + vPosition.y * m_matView._22 + vPosition.z * m_matView._32);
    m_matView._43 = -(vPosition.x * m_matView._13 + vPosition.y * m_matView._23 + vPosition.z * m_matView._33);

    return m_matView;
}

TMatrix LCamera::CreatePerspectiveFov(float fovy, float Aspect, float fNearPlane, float fFarPlane)
{
    float    h, w, Q;
    h = 1 / tan(fovy * 0.5f);
    w = h / Aspect;
    Q = fFarPlane / (fFarPlane - fNearPlane);
    m_matProj._44 = 0.0f;
    m_matProj._11 = w;
    m_matProj._22 = h;
    m_matProj._33 = Q;
    m_matProj._43 = -Q * fNearPlane;
    m_matProj._34 = 1;

    return m_matProj;
}
void LCamera::CreateOrthographic(const int viewWide, const int viewHeight, float nearZ, float FarZ)
{
    

    m_matOrthoProjection = ::XMMatrixOrthographicLH(viewWide, viewHeight, nearZ, FarZ);

}

void LCamera::UpdateVector()
{
    m_vLook.x = m_matView._13;
    m_vLook.y = m_matView._23;
    m_vLook.z = m_matView._33;
    m_vUp.x = m_matView._12;
    m_vUp.y = m_matView._22;
    m_vUp.z = m_matView._32;
    m_vRight.x = m_matView._11;
    m_vRight.y = m_matView._21;
    m_vRight.z = m_matView._31;

    m_Frustum.Create(m_matView, m_matProj);
}

// 직교 투영용 Create
void LCamera::Create(TVector3 pos, TVector2 size)
{
    m_vCameraPos = pos;
    m_dwWindowWidth = size.x;
    m_dwWindowHeight = size.y;
    m_matView._41 = -m_vCameraPos.x;
    m_matView._42 = -m_vCameraPos.y;
    m_matView._43 = -m_vCameraPos.z;
    m_matOrthoProjection._11 = 2.0f / ((float)m_dwWindowWidth);
    m_matOrthoProjection._22 = 2.0f / ((float)m_dwWindowHeight);

    D3DXMatrixIdentity(&m_matWorld);
}

bool LCamera::Init()
{
    return true;
}

bool LCamera::Frame()
{
    return true;
}

bool LCamera::Render()
{
    return true;
}

bool LCamera::Release()
{
    return true;
}