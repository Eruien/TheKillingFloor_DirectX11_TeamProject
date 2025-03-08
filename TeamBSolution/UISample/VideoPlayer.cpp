#include "VideoPlayer.h"


VideoPlayer::VideoPlayer(HWND* hWnd,
    ComPtr< ID3D11Device> pDevice,
    ComPtr< ID3D11DeviceContext> pContext,
    ComPtr<IDXGISwapChain> pSwapChain) : m_hWnd(hWnd), m_pMediaSource(nullptr), m_pSourceReader(nullptr),
    m_pDevice(pDevice), m_pContext(pContext), m_pSwapChain(pSwapChain) {}

VideoPlayer::~VideoPlayer() {
    // Stop();
     //if (m_pMediaSource)
     //    m_pMediaSource->Release();
     //if (m_pSourceReader)
     //    m_pSourceReader->Release();
     //if (m_pSwapChain)
     //    m_pSwapChain->Release();
     //if (m_pContext)
     //    m_pContext->Release();
     //if (m_pDevice)
     //    m_pDevice->Release();
}
HRESULT VideoPlayer::Initialize(const wchar_t* fileName) {
    HRESULT hr = InitDirectX();
    if (FAILED(hr)) {
        std::cerr << "Failed to initialize DirectX" << std::endl;
        return hr;
    }

    hr = InitMediaFoundation();
    if (FAILED(hr)) {
        std::cerr << "Failed to initialize Media Foundation" << std::endl;
        return hr;
    }

    hr = CreateMediaSource(fileName);
    if (FAILED(hr)) {
        std::cerr << "Failed to create media source" << std::endl;
        return hr;
    }

    return S_OK;
}
HRESULT VideoPlayer::Play() {
    if (!m_pMediaSource || !m_pSourceReader) {
        std::cerr << "Media source or source reader is not initialized" << std::endl;
        return E_FAIL;
    }

    // Start reading samples
    HRESULT hr = m_pSourceReader->ReadSample(
        MF_SOURCE_READER_FIRST_VIDEO_STREAM,
        0,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    );

    return hr;
}

HRESULT VideoPlayer::Stop() {
    if (!m_pMediaSource || !m_pSourceReader) {
        std::cerr << "Media source or source reader is not initialized" << std::endl;
        return E_FAIL;
    }

    // Stop reading samples
    HRESULT hr = m_pSourceReader->Flush(MF_SOURCE_READER_FIRST_VIDEO_STREAM);

    return hr;
}

HRESULT VideoPlayer::InitDirectX() {
    // Initialize DirectX here
    // Example code:
    HRESULT hr = S_OK;

    // Create D3D11 device and context
  /*  D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
    hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevels, 1, D3D11_SDK_VERSION, &m_pDevice, nullptr, &m_pContext);
    if (FAILED(hr)) {
        std::cerr << "Failed to create D3D11 device and context" << std::endl;
        return hr;
    }*/

    return hr;
}

HRESULT VideoPlayer::InitMediaFoundation() {
    // Initialize Media Foundation here
    HRESULT hr = MFStartup(MF_VERSION);
    if (FAILED(hr)) {
        std::cerr << "Failed to initialize Media Foundation" << std::endl;
        return hr;
    }

    return hr;
}

HRESULT VideoPlayer::CreateMediaSource(const wchar_t* fileName) {
    // Create media source from file
    HRESULT hr = S_OK;

    // Create IMFAttributes object
    ComPtr<IMFAttributes> pAttributes;
    hr = MFCreateAttributes(&pAttributes, 1);
    if (FAILED(hr)) {
        std::cerr << "Failed to create IMFAttributes object" << std::endl;
        return hr;
    }

    // Set MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING attribute
    hr = pAttributes->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, TRUE);
    if (FAILED(hr)) {
        std::cerr << "Failed to set MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING attribute" << std::endl;
        return hr;
    }

    // Create source reader
    hr = MFCreateSourceReaderFromURL(fileName, pAttributes.Get(), &m_pSourceReader);
    if (FAILED(hr)) {
        std::cerr << "Failed to create source reader" << std::endl;
        return hr;
    }

    // Retrieve media source
    hr = m_pSourceReader->GetServiceForStream(
        MF_SOURCE_READER_FIRST_VIDEO_STREAM,
        GUID_NULL,
        IID_PPV_ARGS(&m_pMediaSource)
    );
    if (FAILED(hr)) {
        std::cerr << "Failed to retrieve media source" << std::endl;
        return hr;
    }

    return hr;
}
