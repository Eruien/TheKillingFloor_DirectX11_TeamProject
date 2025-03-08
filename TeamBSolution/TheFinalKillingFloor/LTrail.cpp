#include "LTrail.h"
#include "LGlobal.h"

void LTrail::RenderTrail(TVector3* localSwordLow, TVector3* localSwordHigh, TMatrix* matSwordParent)
{
    m_TimerStart += LGlobal::g_fSPF;

    if (m_TimerStart > m_TimerEnd)
    {
        m_TimerStart = 0.0f;
        m_iVertexCount += 2;
    }

    if (m_iVertexCount > m_iTrailCountSize)
    {
        m_iVertexCount = 0;
    }

    D3DXVec3TransformCoord(&m_VertexList[m_iVertexCount].p, localSwordLow, matSwordParent);
    D3DXVec3TransformCoord(&m_VertexList[m_iVertexCount + 1].p, localSwordHigh, matSwordParent);

    for (int i = 0; i < m_iVertexCount; i += 2)
    {
        m_VertexList[i].t = { float(i) / (float(m_iVertexCount) - 2), 0.0f };
        m_VertexList[i + 1].t = { float(i) / (float(m_iVertexCount) - 2), 1.0f };
    }

    LGlobal::g_pImmediateContext->UpdateSubresource(m_pVertexBuffer.Get(), 0, NULL, m_VertexList.data(), 0, 0);
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    LGlobal::g_pImmediateContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
    SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    Render();
}

void LTrail::InterpolRenderTrail(TVector3* localSwordLow, TVector3* localSwordHigh, TMatrix* matSwordParent)
{
    m_TimerStart += LGlobal::g_fSPF;

    if (m_TimerStart > m_TimerEnd)
    {
        m_TimerStart = 0.0f;
        m_iVertexCount += 2;
    }

    if (m_iVertexCount > m_iTrailCountSize)
    {
        m_VertexList.clear();
        m_VertexList.resize(m_iTrailVertexSize);
        m_iVertexCount = 0;
    }

    if (m_iVertexCount - 2 < 0)
    {
        m_iCatmullRomIndex[0] = m_iTrailCountSize - 1;
    }
    else
    {
        m_iCatmullRomIndex[0] = m_iVertexCount - 2;
    }

    m_iCatmullRomIndex[1] = m_iVertexCount;

    if (m_iVertexCount + 2 > m_iTrailCountSize)
    {
        m_iCatmullRomIndex[2] = 0;
        m_iCatmullRomIndex[3] = 2;
    }
    else
    {
        m_iCatmullRomIndex[2] = m_iVertexCount + 2;
        m_iCatmullRomIndex[3] = m_iVertexCount + 4;
    }

    if (m_iVertexCount + 4 > m_iTrailCountSize)
    {
        m_iCatmullRomIndex[3] = 0;
    }
    else
    {
        m_iCatmullRomIndex[3] = m_iVertexCount + 4;
    }

    for (int i = 0; i < 1; i++)
    {
        D3DXVec3CatmullRom(&m_VertexList[m_iVertexCount].p,
            &m_VertexList[m_iCatmullRomIndex[0]].p,
            &m_VertexList[m_iCatmullRomIndex[1]].p,
            &m_VertexList[m_iCatmullRomIndex[2]].p,
            &m_VertexList[m_iCatmullRomIndex[3]].p,
            0.5);

        D3DXVec3CatmullRom(&m_VertexList[m_iVertexCount + 1].p,
            &m_VertexList[m_iCatmullRomIndex[0] + 1].p,
            &m_VertexList[m_iCatmullRomIndex[1] + 1].p,
            &m_VertexList[m_iCatmullRomIndex[2] + 1].p,
            &m_VertexList[m_iCatmullRomIndex[3] + 1].p,
            0.5);
    }

    D3DXVec3TransformCoord(&m_VertexList[m_iVertexCount].p, localSwordLow, matSwordParent);
    D3DXVec3TransformCoord(&m_VertexList[m_iVertexCount + 1].p, localSwordHigh, matSwordParent);

    for (int i = 0; i < m_iVertexCount; i += 2)
    {
        m_VertexList[i].t = { float(i) / (float(m_iVertexCount) - 2), 0.0f };
        m_VertexList[i + 1].t = { float(i) / (float(m_iVertexCount) - 2), 1.0f };
    }

    LGlobal::g_pImmediateContext->UpdateSubresource(m_pVertexBuffer.Get(), 0, NULL, m_VertexList.data(), 0, 0);
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    LGlobal::g_pImmediateContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
    SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    Render();
}

bool LTrail::CreateVertexData()
{
	// 업데이트시 pnct값을 넣어주기 때문에 구조체에 값을 넣어주지 않아도 됨
	m_VertexList.resize(m_iTrailVertexSize);
	
	return true;
}

bool LTrail::CreateIndexData()
{
	int indexVertexSize = (m_iTrailVertexSize - 2) * 3;
	m_IndexList.resize(indexVertexSize);

	int squareSize = (indexVertexSize / 6) * 2;

	for (int i = 0; i < squareSize; i += 2)
	{
		m_IndexList[(i * 3)] = i + 3;
		m_IndexList[(i * 3) + 1] = i + 1;
		m_IndexList[(i * 3) + 2] = i;

		m_IndexList[(i * 3) + 3] = i + 2;
		m_IndexList[(i * 3) + 4] = i + 3;
		m_IndexList[(i * 3) + 5] = i;
	}

	return true;
}

