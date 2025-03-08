#include "LSpriteObj.h"

void LSpriteObj::Load(
	ID3D11Device* m_pDevice,
	ID3D11DeviceContext* m_pImmediateContext,
	LSpriteInfo info)
{
	Set(m_pDevice, m_pImmediateContext);
	SetScale(info.vScale);
	SetPos(info.vPos);
	Create(info.shaderFile, info.textureFile);
}

bool LSpriteTexture::Init()
{
    LPlaneObj::Init();
    LTexture* tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/1.png");
    m_TexList.push_back(tex);
    tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/2.png");
    m_TexList.push_back(tex);
    tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/3.png");
    m_TexList.push_back(tex);
    tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/4.png");
    m_TexList.push_back(tex);
    tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/5.png");
    m_TexList.push_back(tex);
    tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/6.png");
    m_TexList.push_back(tex);
    tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/7.png");
    m_TexList.push_back(tex);
    tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/8.png");
    m_TexList.push_back(tex);
    tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/9.png");
    m_TexList.push_back(tex);
   
    m_OffsetTime = 3.0f / m_TexList.size();

    return true;
}

bool LSpriteTexture::Frame()
{
    LPlaneObj::Frame();

    m_GameTimer += g_fSPF;

    if (m_TexList[m_AniIndex] != nullptr)
    {
        if (m_GameTimer >= m_OffsetTime)
        {
            m_AniIndex++;
            if (m_AniIndex >= m_TexList.size())
            {
                m_AniIndex = 0;
            }
            m_GameTimer = m_GameTimer - m_OffsetTime;
        }
    }

    return true;
}

bool LSpriteTexture::Render()
{
    PreRender();
    m_TexList[m_AniIndex]->Apply(m_pImmediateContext);
    PostRender();

    return true;
}

bool LSpriteTexture::Release()
{
    LPlaneObj::Release();
    LManager<LTexture>::GetInstance().Release();
    m_TexList.clear();
    return true;
}

bool LSpriteUV::Init()
{
    LPlaneObj::Init();

    LRectUV rectUV;
    LVector uv;
    float offset = 1.0f / 4.0f;

    for (int i = 0; i < 4; i++)
    {
        uv.x = i * offset;
        for (int i = 0; i < 4; i++)
        {
            uv.y = i * offset;
            rectUV.m_Min = uv;

            rectUV.m_Max.x = uv.x + offset;
            rectUV.m_Max.y = uv.y + offset;
            m_UVList.push_back(rectUV);
        }
    }

    m_OffsetTime = 3.0f / m_UVList.size();
    return true;
}

bool LSpriteUV::Frame()
{
    LPlaneObj::Frame();

    m_GameTimer += g_fSPF;
 
    if (m_GameTimer >= m_OffsetTime)
    {
        m_AniIndex++;
        if (m_AniIndex >= m_UVList.size())
        {
            m_AniIndex = 0;
        }
        m_GameTimer = m_GameTimer - m_OffsetTime;
    }
    
    return true;
}

bool LSpriteUV::Render()
{
    LRectUV rectUV = m_UVList[m_AniIndex];
    m_VertexList[0].t.x = rectUV.m_Min.x;  m_VertexList[0].t.y = rectUV.m_Min.y;
    m_VertexList[1].t.x = rectUV.m_Max.x;  m_VertexList[1].t.y = rectUV.m_Min.y;
    m_VertexList[2].t.x = rectUV.m_Min.x;  m_VertexList[2].t.y = rectUV.m_Max.y;

    m_VertexList[3].t.x = rectUV.m_Min.x;  m_VertexList[3].t.y = rectUV.m_Max.y;
    m_VertexList[4].t.x = rectUV.m_Max.x;  m_VertexList[4].t.y = rectUV.m_Min.y;
    m_VertexList[5].t.x = rectUV.m_Max.x;  m_VertexList[5].t.y = rectUV.m_Max.y;

    m_pImmediateContext->UpdateSubresource(
        m_pVertexBuffer, 0, nullptr, &m_VertexList.at(0), 0, 0);
    PreRender();
    PostRender();
    return true;
}

bool LSpriteUV::Release()
{
    return true;
}
