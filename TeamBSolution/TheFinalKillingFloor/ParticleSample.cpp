#include "Sample.h"

void Sample::CreateBlendState()
{
    D3D11_BLEND_DESC bsd = { 0, };
    bsd.RenderTarget[0].BlendEnable = true;
    bsd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bsd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bsd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

    bsd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bsd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    bsd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bsd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    m_pDevice->CreateBlendState(&bsd, &m_AlphaBlend);
}

bool Sample::Init()
{
    CreateBlendState();

    m_pMapObj = new LPlaneObj;
    m_pMapObj->Set(m_pDevice, m_pImmediateContext);
    m_pMapObj->SetScale(LVector(g_fMapSizeX, g_fMapSizeY, 1.0f));
    m_pMapObj->Create(L"../../res/PVShader.hlsl", L"../../res/kirbymap.png");
    
    m_pPlayer = new LPlayer;
    m_pPlayer->Set(m_pDevice, m_pImmediateContext);
    m_pPlayer->SetBox(m_pPlayer->m_vPosition, m_pPlayer->m_vScale.x * 2, m_pPlayer->m_vScale.x * 2, 1.0f);
    m_pPlayer->SetScale(LVector(30.769230f * 30.0f, 23.076923f * 30.0f, 1.0f));
    m_pPlayer->Create(L"../../res/PVShader.hlsl", L"../../res/kirby.png");
    m_MainCamera.Create(m_pPlayer->m_vPosition, LVector2(m_WindowWidth, m_WindowHeight));

    m_TexObj = new LSpriteTexture;
    m_TexObj->Init();
    LSpriteInfo info1;
    info1.vPos = { randstep(-g_fMapSizeX, g_fMapSizeX), randstep(-g_fMapSizeY, g_fMapSizeY), 1.0f };
    info1.vScale = { 30.0f, 30.0f, 1.0f };
    info1.shaderFile = L"../../res/PVShader.hlsl";
    info1.textureFile = L"../../res/ui/0.png";
    m_TexObj->Load(m_pDevice, m_pImmediateContext, info1);
    m_SpriteList.insert(std::make_pair(0, m_TexObj));

    m_UVObj = new LSpriteUV;
    m_UVObj->Init();
    LSpriteInfo info2;
    info2.vPos = { 0.0f, 0.0f, 1.0f };
    info2.vScale = { 30.0f, 30.0f, 1.0f };
    info2.shaderFile = L"../../res/PVShader.hlsl";
    info2.textureFile = L"../../res/103.tga";
    m_UVObj->Load(m_pDevice, m_pImmediateContext, info2);
    m_SpriteList.insert(std::make_pair(1, m_UVObj));

    return true;
}

bool Sample::Frame()
{
    m_pMapObj->Frame();
    m_pPlayer->Frame();

    if (LInput::GetInstance().m_dwKeyState[VK_LBUTTON] >= DWORD(KeyState::KEY_UP))
    {
        LVector mouse = LInput::GetInstance().GetWorldPos(
            m_WindowWidth, m_WindowHeight,
            m_MainCamera.m_vCameraPos.x, m_MainCamera.m_vCameraPos.y);
        LParticle info = { 0, };
        info.m_isLife = true;
        info.m_iIndex = rand() % m_SpriteList.size();
        info.m_vPos = mouse;
        info.m_vScale = { 50.0f, 50.0f, 50.0f };
        info.m_MaxFrame = m_SpriteList[info.m_iIndex]->GetMaxSize();
        info.m_OffsetTime = 3.0f / info.m_MaxFrame;
        m_ParticleList.push_back(info);
    }
    
    for (auto iter = m_ParticleList.begin(); iter != m_ParticleList.end(); iter++)
    {
        LParticle& info = *iter;
        info.Frame();

        if (info.m_isLife == false)
        {
            //iter = m_ParticleList.erase(iter);
        }
        else
        {
            //iter++;
        }
    }
    
    return true;
}

bool Sample::Render()
{
    m_pImmediateContext->OMSetBlendState(m_AlphaBlend, 0, -1);

    m_pMapObj->SetMatrix(nullptr, &m_MainCamera.m_matView, &m_MainCamera.m_matOrthoProjection);
    m_pMapObj->Render();
    m_pPlayer->SetMatrix(nullptr, &m_MainCamera.m_matView, &m_MainCamera.m_matOrthoProjection);
    m_pPlayer->Render();
    m_MainCamera.m_vCameraPos = m_pPlayer->m_vPosition;

    for (auto iter = m_ParticleList.begin(); iter != m_ParticleList.end(); iter++)
    {
        LParticle& info = *iter;
        LSpriteObj* SpriteObj = m_SpriteList[info.m_iIndex];
        SpriteObj->SetPos(info.m_vPos);
        SpriteObj->SetScale(info.m_vScale);
        SpriteObj->m_AniIndex = info.m_AniIndex;
        SpriteObj->Frame();
        SpriteObj->SetMatrix(nullptr, &m_MainCamera.m_matView, &m_MainCamera.m_matOrthoProjection);
        SpriteObj->Render();
    }

    return true;
}

bool Sample::Release()
{
    m_pMapObj->Release();
    delete m_pMapObj;
    m_pMapObj = nullptr;

    m_pPlayer->Release();

    for (auto npc : m_NpcList)
    {
        npc->Release();
        delete npc;
    }
    m_NpcList.clear();

    m_AlphaBlend->Release();
    m_TexObj->Release();
    return true;
}

Sample::~Sample() {}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR IpCmdLine, int nCmdShow)
{
    Sample win;
    win.SetRegisterWindowClass(hInstance);
    win.SetCreateWindow(L"√¢¿Ã¥Ÿ", 800, 600);
    win.Run();
}


