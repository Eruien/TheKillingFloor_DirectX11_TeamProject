#pragma once
#include "LStd.h"

class LDxRT
{
public:
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
	ComPtr<ID3D11ShaderResourceView> m_pSRV;
	ComPtr<ID3D11ShaderResourceView> m_pDsvSRV;// ±Ì¿Ãπˆ∆€ ∏Æº“Ω∫∫‰
	ComPtr<ID3D11Texture2D> m_pTexture;
	D3D11_DEPTH_STENCIL_VIEW_DESC			m_DepthStencilDesc;
	D3D11_TEXTURE2D_DESC					m_TexDesc;
	DXGI_FORMAT								m_DSFormat;
	D3D11_VIEWPORT			m_vp;
	TMatrix				m_matProj;

	// Store the old render targets
	D3D11_VIEWPORT			m_vpOld[D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX];
	UINT					m_nViewPorts;
	ID3D11RenderTargetView* m_pOldRTV;
	ID3D11DepthStencilView* m_pOldDSV;
public:
	HRESULT	Create(FLOAT Width = 1024, FLOAT Height = 1024);
	void	Set(FLOAT TopLeftX, FLOAT TopLeftY, FLOAT Width, FLOAT Height,
		FLOAT MinDepth = 0, FLOAT MaxDepth = 1);
	HRESULT UpdateDepthStencilView(UINT dwWidth, UINT dwHeight);
	void	Apply(ID3D11DeviceContext* pImmediateContext,
		ID3D11RenderTargetView* pRTV = NULL,
		ID3D11DepthStencilView* pDepthStencilView = NULL,
		bool bDepthClear = true, bool bStencilClear = true);
	bool	Begin(TVector4 vColor,
		bool bTargetClear = true, bool bDepthClear = true, bool bStencilClear = true);
	bool	Clear(TVector4 vColor,
		bool bTargetClear = true, bool bDepthClear = true, bool bStencilClear = true);
	bool	End();
	LDxRT();
	~LDxRT() {};
};

