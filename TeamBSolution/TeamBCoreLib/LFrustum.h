#pragma once
#include "LStd.h"

enum class CullResult {
    SPANNING,
    INSIDE,
    OUTSIDE
};

//class xPlane
//{
//public:
//    float fa, fb, fc, fd;
//public:
//    void Normalize();
//    void CreatePlane(TVector3 v0, TVector3 v1, TVector3 v2);
//    void CreatePlane(TVector3 v0, TVector3 vNormal);
//};

class LFrustum
{
public:
    TVector3  m_vFrustum[8];
    T_PLANE    m_xPlane[6];
public:
    void Create(TMatrix matView, TMatrix matProj);
    CullResult CheckOBBInPlane(T_BOX& box);
};
