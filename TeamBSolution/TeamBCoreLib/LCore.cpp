#include "LCore.h"
#include "LGlobal.h"
#include "../TheFinalKillingFloor/UI/UIManager.h"
bool LCore::Init() { return true; }
bool LCore::Frame() { return true; }
bool LCore::Render() { return true; }
bool LCore::Release() { return true; }

void LCore::CreateBlendState()
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

    m_pDevice->CreateBlendState(&bsd, m_AlphaBlend.GetAddressOf());
}

void LCore::CreateSamplerState()
{
    D3D11_SAMPLER_DESC descSampler;
    descSampler.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

    descSampler.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    descSampler.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    descSampler.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

    descSampler.MipLODBias = 0;
    descSampler.MaxAnisotropy = 16;

    descSampler.ComparisonFunc = D3D11_COMPARISON_NEVER;

    descSampler.BorderColor[0] = 1.0f;
    descSampler.BorderColor[1] = 0.0f;
    descSampler.BorderColor[2] = 0.0f;
    descSampler.BorderColor[3] = 1.0f;

    descSampler.MinLOD = 0;
    descSampler.MaxLOD = D3D11_FLOAT32_MAX;

    m_pDevice->CreateSamplerState(&descSampler, m_pSamplerState.GetAddressOf());
}

void LCore::CreateClampState()
{
    D3D11_SAMPLER_DESC descSampler;
    descSampler.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

    descSampler.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    descSampler.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    descSampler.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

    descSampler.MipLODBias = 0;
    descSampler.MaxAnisotropy = 16;

    descSampler.ComparisonFunc = D3D11_COMPARISON_NEVER;

    descSampler.BorderColor[0] = 1.0f;
    descSampler.BorderColor[1] = 0.0f;
    descSampler.BorderColor[2] = 0.0f;
    descSampler.BorderColor[3] = 1.0f;

    descSampler.MinLOD = 0;
    descSampler.MaxLOD = D3D11_FLOAT32_MAX;

    m_pDevice->CreateSamplerState(&descSampler, m_pClampState.GetAddressOf());
}

void LCore::CreateDepthStencilState()
{
    HRESULT hr;
    // 깊이버퍼 상태값 세팅
    D3D11_DEPTH_STENCIL_DESC dsDescDepth = { 0, };
    dsDescDepth.DepthEnable = TRUE;
    // 전부다 그린다
    dsDescDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    // 해당 픽셀의 깊이 연산을 하여 출력여부를 판단한다.
    // 지형의 깊이 값 >=  깊이버퍼의 픽셀값(1.0f)
    // 초기화 하는 깊이 보다 작으면 그려지게
    dsDescDepth.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    //Stencil
    dsDescDepth.StencilEnable = FALSE;
    dsDescDepth.StencilReadMask = 1;
    dsDescDepth.StencilWriteMask = 1;
    dsDescDepth.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    dsDescDepth.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
    dsDescDepth.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDescDepth.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    // 디폴트 값
    dsDescDepth.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    dsDescDepth.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDescDepth.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDescDepth.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

    // 이쪽엔  dsDescDepth.DepthEnable이 true로 들어갔음
    if (FAILED(hr = m_pDevice->CreateDepthStencilState(&dsDescDepth,
        m_pDepthStencilState.GetAddressOf())))
    {
        return;
    }

    dsDescDepth.DepthEnable = FALSE;

    // 이쪽엔  dsDescDepth.DepthEnable이 false로 들어갔음
    if (FAILED(hr = m_pDevice->CreateDepthStencilState(&dsDescDepth,
        m_pDepthStencilStateDisable.GetAddressOf())))
    {
        return;
    }

    return;
}

void LCore::CreateRasterizerState()
{
    HRESULT hr;
    D3D11_RASTERIZER_DESC rd;
    ZeroMemory(&rd, sizeof(rd));
    rd.CullMode = D3D11_CULL_NONE;
    rd.FillMode = D3D11_FILL_SOLID;
    hr = m_pDevice->CreateRasterizerState(&rd, m_pRSSolid.GetAddressOf());
    rd.FillMode = D3D11_FILL_WIREFRAME;
    hr = m_pDevice->CreateRasterizerState(&rd, m_pRSWireFrame.GetAddressOf());
}

bool LCore::EngineInit()
{
    LDevice::Init();
    LGlobal::g_pDevice = m_pDevice;
    LGlobal::g_pImmediateContext = m_pImmediateContext;

    CreateBlendState();
    CreateSamplerState();
    CreateClampState();
    CreateDepthStencilState();
    CreateRasterizerState();

    m_Gametimer = std::make_shared<LTimer>();
    m_Gametimer->Init();

    m_pDefaultCamera = std::make_shared<LCamera>();
    m_pDefaultCamera->Init();
    m_pDefaultCamera->CreateLookAt({ 0.0f, 600.0f, -300.0f }, { 0.0f, 0.0f, 1.0f });
    //투영 행렬
    m_pDefaultCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);
    

    LGlobal::g_pMainCamera = m_pDefaultCamera.get();

    m_pDefaultUICamera = std::make_shared<UICamera>();
    m_pDefaultUICamera->CreateLookAt({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
    m_pDefaultUICamera->m_fCameraPitch = 0.0f;
    m_pDefaultUICamera->CreateOrthographic((float)LGlobal::g_WindowWidth, (float)LGlobal::g_WindowHeight, -1, 1); // 직교투영행렬

    LGlobal::g_pUICamera = m_pDefaultUICamera.get();


    LGlobal::g_hInstance = m_hInstance;

    LInput::GetInstance().Init();
    LWrite::GetInstance().Init();
    LWrite::GetInstance().Create(m_pSwapChain.Get());

    LGlobal::g_pDepthStencilState = m_pDepthStencilState;
    LGlobal::g_pDepthStencilStateDisable = m_pDepthStencilStateDisable;

    Init();
    return true;
}

bool LCore::EngineFrame()
{
    LDevice::Frame();
    m_Gametimer->Frame();
    LInput::GetInstance().Frame();
    LWrite::GetInstance().Frame();
    LGlobal::g_pMainCamera->Frame();
    LGlobal::g_pUICamera->Frame();
    LSoundMgr::GetInstance().Frame();
    Frame();
    return true;
}

bool LCore::EngineRender()
{
    m_pImmediateContext->OMSetBlendState(m_AlphaBlend.Get(), nullptr, 0xFFFFFFFF);
    LDevice::PreRender();
    m_pImmediateContext->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());
    m_pImmediateContext->PSSetSamplers(1, 1, m_pClampState.GetAddressOf());
    // 스탠실 스테이트 필요 1은 lessequal로 설정했기 때문에 1보다 같거나 작으면 그려지게 했다
    m_pImmediateContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);

    if (g_InputData.bF1Key)
    {
        m_ISWireFrame = !m_ISWireFrame;
    }

    m_pImmediateContext->RSSetState(m_pRSWireFrame.Get());

    if (m_ISWireFrame)
    {
        m_pImmediateContext->RSSetState(m_pRSWireFrame.Get());
    }
    else
    {
        m_pImmediateContext->RSSetState(m_pRSSolid.Get());
    }

    LWrite::GetInstance().PreRender();
  
    Render();
    LGlobal::g_pMainCamera->Render();
    LGlobal::g_pUICamera->Render();
    m_Gametimer->Render();
    LWrite::GetInstance().Render();
    LInput::GetInstance().Render();
    LWrite::GetInstance().PostRender();
    LDevice::PostRender();
    
    return true;
}

bool LCore::EngineRelease()
{
    LDevice::Release();
    Release();
    m_Gametimer->Release();
    LGlobal::g_pMainCamera->Release();
    LWrite::GetInstance().Release();
    LInput::GetInstance().Release();
    LManager<LTexture>::GetInstance().Release();
    LManager<LShader>::GetInstance().Release();
    return true;
}

bool LCore::Run()
{
    EngineInit();

    MSG msg = { 0 };

    while (msg.message != WM_QUIT)
    {
        if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            EngineFrame();
            EngineRender();
        }
    }

    EngineRelease();

    return true;
}

void LCore::ResizeDevice(UINT width, UINT height)
 {
    HRESULT hr;
    if (m_pDevice == nullptr) return;
    DeleteDxResource();

    m_pImmediateContext->OMSetRenderTargets(0, nullptr, nullptr);
    m_pRenderTargetView.Reset();
    m_pDepthStencilView.Reset();
    if (m_pSwapChain == nullptr) return;
    hr = m_pSwapChain->ResizeBuffers(m_SwapChainDesc.BufferCount,
        width, height, m_SwapChainDesc.BufferDesc.Format, m_SwapChainDesc.Flags);

    if (FAILED(hr))
    {
        MessageBoxA(NULL, "ResizeBuffer Error", "Error Box", MB_OK);
        return;
    }

    m_pSwapChain->GetDesc(&m_SwapChainDesc);

    SetRenderTargetView();
    SetDepthTexture();
    SetDepthStencilView();
    SetViewPort();

    GetClientRect(m_hWnd, &m_rcClient);
    LGlobal::g_WindowWidth = m_WindowWidth = m_rcClient.right;
    LGlobal::g_WindowHeight = m_WindowHeight = m_rcClient.bottom;
    LGlobal::g_pUICamera->CreateOrthographic((float)LGlobal::g_WindowWidth, (float)LGlobal::g_WindowHeight, -1, 1);
    CreateDxResource();

    UIManager::GetInstance().UpdateResolution(LGlobal::g_WindowWidth, LGlobal::g_WindowHeight);

}

bool LCore::DeleteDxResource()
{
    LWrite::GetInstance().DeleteDxResource();
    return true;
}

bool LCore::CreateDxResource()
{
    if (m_pSwapChain)
    {
        IDXGISurface1* pBackBuffer;
        HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface1), (LPVOID*)&pBackBuffer);

        if (SUCCEEDED(hr))
        {
            LWrite::GetInstance().CreateDxResource(pBackBuffer);
        }
    }
    return true;
}

LCore::~LCore() {}