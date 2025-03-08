#pragma once
#include "LWindow.h"

class LDevice : public LWindow
{
public:
	DXGI_SWAP_CHAIN_DESC m_SwapChainDesc;
	// interface
	ComPtr<ID3D11Device> m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext> m_pImmediateContext = nullptr;
	ComPtr<IDXGISwapChain> m_pSwapChain = nullptr;
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView = nullptr;
	ComPtr<IDXGIFactory> m_pGIFactory = nullptr;
	ID3D11Texture2D* m_pbackBuffer = nullptr;
	// depth buffer
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView = nullptr;
	ComPtr<ID3D11Texture2D> m_DepthTexture = nullptr;
	// viewport
	D3D11_VIEWPORT m_ViewPort;
public:
	bool SetDevice();
	bool SetGIFactory();
	bool SetSwapChain();
	bool SetDepthTexture();
	bool SetDepthStencilView();
	bool SetRenderTargetView();
	bool SetViewPort();
public:
	bool Init();
	bool Frame();
	bool PreRender();
	bool PostRender();
	bool Release();
public:
	virtual ~LDevice();
};

