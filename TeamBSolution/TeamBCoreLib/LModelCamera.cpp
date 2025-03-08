#include "LModelCamera.h"
#include "LGlobal.h"
#include "LInput.h"

void LModelCamera::SetTarget(LModel* model)
{
    m_TargetModel = model;
    m_ModelMatrix = model->m_matControl;
}

void LModelCamera::SetTargetPos(TVector3 vPos)
{
    m_vTargetPos = vPos;
};

void LModelCamera::SetViewMatrix(TVector3 vPos, TVector3 vTarget, TVector3 vUp)
{
    m_vCameraPos = vPos;
    m_vTargetPos = vTarget;
    D3DXMatrixLookAtLH(&m_matView, &m_vCameraPos, &m_vTargetPos, &vUp);

    TQuaternion qRotation;
    TVector3 vTrans, vScale, vAxis;
    TMatrix mInvView;
    TQuaternion q;
    float fAngle;

    if (SUCCEEDED(D3DXMatrixDecompose(&vScale, &qRotation, &vTrans, &m_matView)))
    {
        D3DXQuaternionNormalize(&qRotation, &qRotation);
        D3DXQuaternionToAxisAngle(&qRotation, &vAxis, &fAngle);
        // 반드시 정규화 해야 한다.
        D3DXVec3Normalize(&vAxis, &vAxis);

        q.x = sin(fAngle / 2) * vAxis.x;
        q.y = sin(fAngle / 2) * vAxis.y;
        q.z = sin(fAngle / 2) * vAxis.z;
        q.w = cos(fAngle / 2);
    }
    D3DXMatrixInverse(&mInvView, NULL, &m_matView);
    TVector3* pZBasis = (TVector3*)&mInvView._31;

    m_fCameraYaw = atan2f(pZBasis->x, pZBasis->z);
    float fLen = sqrtf(pZBasis->z * pZBasis->z + pZBasis->x * pZBasis->x);
    m_fCameraPitch = -atan2f(pZBasis->y, fLen);
    UpdateVector();
    return;
}

void LModelCamera::Update(TVector4 vDirValue)
{
    TMatrix matRotation;
    D3DXMatrixRotationYawPitchRoll(&matRotation, vDirValue.y, vDirValue.x, vDirValue.z);
    // 기저벡터를 카메라 회전행렬로 변환한다.
    TVector3 vWorldUp, vWorldLook;
    TVector3 vLocalUp = TVector3(0, 1, 0);
    TVector3 vLocalLook = TVector3(0, 0, 1);
    D3DXVec3TransformCoord(&vWorldUp, &vLocalUp, &matRotation);
    D3DXVec3TransformCoord(&vWorldLook, &vLocalLook, &matRotation);
    // 카메라 위치를 디렉션벡터* 반지름크기 만큼 이동시킨다.	
    m_vCameraPos = m_vTargetPos - (vWorldLook * vDirValue.w);
    SetViewMatrix(m_vCameraPos, m_vTargetPos, vWorldUp);
    return;
}

bool LModelCamera::Frame()
{
    if (m_TargetModel == nullptr) return false;

    float x = LInput::GetInstance().m_vOffset.x;
    float y = LInput::GetInstance().m_vOffset.y;
    m_fCameraYaw += DirectX::XMConvertToRadians(x * 0.1f);
    m_fCameraPitch += DirectX::XMConvertToRadians(y * 0.1f);
    DirectX::XMVECTOR gRotation;
    DirectX::XMMATRIX matRotation;
    TMatrix matTemp;
   
    gRotation = DirectX::XMQuaternionRotationRollPitchYaw(0, m_fCameraYaw, 0);
    DirectX::XMVECTOR xmModelPos = DirectX::XMVectorSet(m_TargetModel->m_matControl._41, m_TargetModel->m_matControl._42, m_TargetModel->m_matControl._43, 1.0f);
    matRotation = DirectX::XMMatrixAffineTransformation(DirectX::g_XMOne, DirectX::g_XMZero, gRotation, xmModelPos);

    matTemp = m_ModelMatrix * matRotation;
    m_TargetModel->m_matControl = matTemp;
   
    m_fCameraPitch = max(-m_MaxPitch, min(m_MaxPitch, m_fCameraPitch));

    if (LInput::GetInstance().m_KeyStateOld[DIK_Q] > KEY_PUSH)
    {
        m_fRadius += (LGlobal::g_fSPF * 20.0f);
    }
   
    if (LInput::GetInstance().m_KeyStateOld[DIK_E] > KEY_PUSH)
    {
        m_fRadius += (-LGlobal::g_fSPF * 20.0f);
    }

    Update(TVector4(m_fCameraPitch, m_fCameraYaw, 0.0f, m_fRadius));

    return true;
}

LModelCamera::LModelCamera()
{
}

LModelCamera::~LModelCamera()
{
}

