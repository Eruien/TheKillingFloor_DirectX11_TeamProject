#pragma once
#include "LFrustum.h"

class LCamera
{
public:
    LFrustum m_Frustum;
public:
    TVector3 m_vLook;
    TVector3 m_vUp;
    TVector3 m_vRight;
    float m_fCameraYaw = 0.0f;
    float m_fCameraPitch = 45.0f;
    float m_fCameraRoll = 0.0f;
    float m_fSpeed = 10.0f;
public:
    TMatrix m_matWorld;

    TMatrix m_matView;
    TMatrix m_matOrthoProjection;
    TMatrix m_matProj;
    TVector3 m_vCameraPos;
    TVector3 m_vTargetPos;
    DWORD m_dwWindowWidth;
    DWORD m_dwWindowHeight;
public:
    TMatrix   CreateLookAt(TVector3 vPosition, TVector3 vTarget, TVector3 vUp = { 0,1,0 });
    TMatrix   CreatePerspectiveFov(float fFov, float fAspect, float fNear, float fFar);
    void   CreateOrthographic(const int viewWide, const int viewHeight, float nearZ, float FarZ);
    void UpdateVector();
public:
    virtual bool Init();
    virtual bool Frame();
    virtual bool Render();
    virtual bool Release();
public:
    void Create(TVector3 pos, TVector2 size);
public:
    virtual ~LCamera() {}
};

