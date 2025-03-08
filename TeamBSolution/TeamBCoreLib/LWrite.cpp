#include "LWrite.h"
#include "LGlobal.h"

void LWrite::SetCreateFactory()
{
    HRESULT hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_MULTI_THREADED,
        m_pFactory.GetAddressOf());

    if (FAILED(hr))
    {
        MessageBoxA(NULL, "Create D2D1Factory Error", "Error Box", MB_OK);
    }
}

void LWrite::SetCreateWriteFactory()
{
    HRESULT hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        (IUnknown**)m_pWriteFactory.GetAddressOf());

    if (FAILED(hr))
    {
        MessageBoxA(NULL, "Create WriteFactory Error", "Error Box", MB_OK);
    }
}

void LWrite::SetCreateTextFormat()
{
    HRESULT hr = m_pWriteFactory->CreateTextFormat(
        L"고딕",
        nullptr,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        30,
        L"ko-kr",
        m_ptextFormat.GetAddressOf());

    if (FAILED(hr))
    {
        MessageBoxA(NULL, "Create TextFormat Error", "Error Box", MB_OK);
        return;
    }
}

void LWrite::SetCreateBackBuffer(IDXGISwapChain* pSwapChain)
{
    m_pSwapChain = pSwapChain;

    HRESULT hr = m_pSwapChain->GetBuffer(
        0,
        __uuidof(IDXGISurface1),
        (PVOID*)&m_pbackBuffer
    );

    if (FAILED(hr))
    {
        MessageBoxA(NULL, "Create BackBuffer Error", "Error Box", MB_OK);
        m_pbackBuffer->Release();
        return;
    }
}

void LWrite::SetCreateWriteRT()
{
    D2D1_RENDER_TARGET_PROPERTIES prop = {};
    prop.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
    // 기존에 있던걸 쓰겠다.
    prop.pixelFormat.format = DXGI_FORMAT_UNKNOWN;
    prop.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
    float dpi = GetDpiForWindow(LGlobal::g_hWnd);
    prop.dpiX = dpi;
    prop.dpiY = dpi;
    prop.usage = D2D1_RENDER_TARGET_USAGE_NONE;
    prop.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

    HRESULT hr = m_pFactory->CreateDxgiSurfaceRenderTarget(
        m_pbackBuffer,
        &prop,
        m_pRenderTarget.GetAddressOf());

    if (FAILED(hr))
    {
        MessageBoxA(NULL, "Create WriteRT Error", "Error Box", MB_OK);
        m_pbackBuffer->Release();
        return;
    }

    hr = m_pRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::Yellow),
        m_pSolidColorBrush.GetAddressOf());

    if (FAILED(hr))
    {
        MessageBoxA(NULL, "Create SolidColorBrush Error", "Error Box", MB_OK);
        m_pbackBuffer->Release();
        return;
    }

    m_pbackBuffer->Release();
}

void LWrite::SetCreateWriteRT(IDXGISurface1* pBackBuffer)
{
    m_pbackBuffer = pBackBuffer;
    D2D1_RENDER_TARGET_PROPERTIES prop = {};
    prop.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
    // 기존에 있던걸 쓰겠다.
    prop.pixelFormat.format = DXGI_FORMAT_UNKNOWN;
    prop.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
    float dpi = GetDpiForWindow(LGlobal::g_hWnd);
    prop.dpiX = dpi;
    prop.dpiY = dpi;
    prop.usage = D2D1_RENDER_TARGET_USAGE_NONE;
    prop.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

    HRESULT hr = m_pFactory->CreateDxgiSurfaceRenderTarget(
        m_pbackBuffer,
        &prop,
        m_pRenderTarget.GetAddressOf());

    if (FAILED(hr))
    {
        MessageBoxA(NULL, "Create WriteRT Error", "Error Box", MB_OK);
        m_pbackBuffer->Release();
        return;
    }

    hr = m_pRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::Yellow),
        m_pSolidColorBrush.GetAddressOf());

    if (FAILED(hr))
    {
        MessageBoxA(NULL, "Create SolidColorBrush Error", "Error Box", MB_OK);
        m_pbackBuffer->Release();
        return;
    }

    m_pbackBuffer->Release();
}

bool LWrite::Create(IDXGISwapChain* pSwapChain)
{
    SetCreateFactory();
    SetCreateWriteFactory();
    SetCreateTextFormat();
    SetCreateBackBuffer(pSwapChain);
    SetCreateWriteRT();

    return true;
}

void LWrite::AddText(std::wstring text, float x = 0.0f, float y = 0.0f, D2D1::ColorF color)
{
    LTextData textData;
    textData.text = text;
    textData.layout = { x, y, float(LGlobal::g_WindowWidth), float(LGlobal::g_WindowHeight) };
    textData.color = color;
    m_TextList.push_back(textData);
}


bool LWrite::Init()
{
    return true;
}

bool LWrite::Frame()
{
    return true;
}

bool LWrite::PreRender()
{
    if (m_pRenderTarget)
    {
        m_pRenderTarget->BeginDraw();
        m_pRenderTarget->SetTransform(D2D1::IdentityMatrix());
        return true;
    }

    return false;
}

bool LWrite::Render()
{

    for (int i = 0; i < m_TextList.size(); i++)
    {
        std::wstring text = m_TextList[i].text;
        D2D1_RECT_F layout = m_TextList[i].layout;
        m_pSolidColorBrush->SetColor(m_TextList[i].color);
        m_pSolidColorBrush->SetOpacity(1.0f);
        m_pRenderTarget->DrawText(text.c_str(), text.size(), m_ptextFormat.Get(), &layout, m_pSolidColorBrush.Get());
    }

    return true;
}

bool LWrite::PostRender()
{
    m_pRenderTarget->EndDraw();
    m_TextList.clear();
    return true;
}

bool LWrite::Release()
{
    return true;
}

bool LWrite::DeleteDxResource()
{
    m_pRenderTarget.Reset();
    m_pSolidColorBrush.Reset();

    return true;
}

bool LWrite::CreateDxResource(IDXGISurface1* pBackBuffer)
{
    if (pBackBuffer)
    {
        SetCreateWriteRT(pBackBuffer);
    }
    return true;
}


