#include "LAnimation.h"
#include "LGlobal.h"

void LAnimation::SetUVAnimation(std::wstring filePath, int spriteCount, float animationTime)
{
    m_AnimationTexture = LManager<LTexture>::GetInstance().Load(filePath);

    RectUV rectUV;
    TVector3 uv;

    float offset = 1.0f / float(spriteCount);

    for (int i = 0; i < spriteCount; i++)
    {
        uv.x = i * offset;
        uv.y = 0 * offset;
        rectUV.m_Min = uv;

        rectUV.m_Max.x = uv.x + offset;
        rectUV.m_Max.y = 1.0f;
        m_UVList.push_back(rectUV);

    }

    m_OffsetTime = animationTime / m_UVList.size();
}

void LAnimation::SetUVPosition(std::vector<SimpleVertex>& vertexList)
{
    RectUV rectUV = m_UVList[m_AnimationIndex];

    vertexList[0].t.x = rectUV.m_Min.x;  vertexList[0].t.y = rectUV.m_Min.y;
    vertexList[1].t.x = rectUV.m_Max.x;  vertexList[1].t.y = rectUV.m_Min.y;
    vertexList[2].t.x = rectUV.m_Min.x;  vertexList[2].t.y = rectUV.m_Max.y;

    vertexList[3].t.x = rectUV.m_Min.x;  vertexList[3].t.y = rectUV.m_Max.y;
    vertexList[4].t.x = rectUV.m_Max.x;  vertexList[4].t.y = rectUV.m_Min.y;
    vertexList[5].t.x = rectUV.m_Max.x;  vertexList[5].t.y = rectUV.m_Max.y;
}

void LAnimation::SetUVPositionReverse(std::vector<SimpleVertex>& vertexList)
{
    RectUV rectUV = m_UVList[m_AnimationIndex];

    vertexList[0].t.x = rectUV.m_Max.x;  vertexList[0].t.y = rectUV.m_Min.y;
    vertexList[1].t.x = rectUV.m_Min.x;  vertexList[1].t.y = rectUV.m_Min.y;
    vertexList[2].t.x = rectUV.m_Max.x;  vertexList[2].t.y = rectUV.m_Max.y;

    vertexList[3].t.x = rectUV.m_Max.x;  vertexList[2].t.y = rectUV.m_Max.y;
    vertexList[4].t.x = rectUV.m_Min.x;  vertexList[1].t.y = rectUV.m_Min.y;
    vertexList[5].t.x = rectUV.m_Min.x;  vertexList[5].t.y = rectUV.m_Max.y;
}

bool LAnimation::Init()
{
    return true;
}

bool LAnimation::Frame()
{
    m_AnimationElapsed += LGlobal::g_fSPF;

    if (m_AnimationElapsed >= m_OffsetTime)
    {
        m_AnimationIndex++;

        if (m_AnimationIndex >= m_UVList.size())
        {
            m_AnimationIndex = 0;
            m_AnimationElapsed = 0;
        }

        m_AnimationElapsed -= m_OffsetTime;
    }

    return true;
}

bool LAnimation::Render(std::vector<SimpleVertex>& vertexList, ID3D11DeviceContext* pImmediateContext, ID3D11Buffer* m_pVertexBuffer)
{
    pImmediateContext->UpdateSubresource(
        m_pVertexBuffer, 0, nullptr, &vertexList.at(0), 0, 0);

    return true;
}



bool LAnimation::Release()
{
    return true;
}

LAnimation::LAnimation() {}
