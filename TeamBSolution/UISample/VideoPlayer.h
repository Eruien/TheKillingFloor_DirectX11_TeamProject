#include <windows.h>
#include "mfapi.h"
#include "mfidl.h"
#include "mfreadwrite.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <iostream>

using namespace Microsoft::WRL;

class VideoPlayer {
public:
    VideoPlayer(HWND* hWnd,
        ComPtr< ID3D11Device> pDevice,
        ComPtr< ID3D11DeviceContext> pContext,
        ComPtr<IDXGISwapChain> pSwapChain);
    ~VideoPlayer();

    HRESULT Initialize(const wchar_t* fileName);
    HRESULT Play();
    HRESULT Stop();

private:
    HWND* m_hWnd;
    IMFMediaSource* m_pMediaSource;
    IMFSourceReader* m_pSourceReader;
    ComPtr<ID3D11Device> m_pDevice;
    ComPtr<ID3D11DeviceContext> m_pContext;
    ComPtr< IDXGISwapChain> m_pSwapChain;

    HRESULT InitDirectX();
    HRESULT InitMediaFoundation();
    HRESULT CreateMediaSource(const wchar_t* fileName);
};