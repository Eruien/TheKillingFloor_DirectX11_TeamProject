#include "LDxRT.h"
#include "LGlobal.h"

LDxRT::LDxRT()
{
	m_pRenderTargetView = nullptr;
	m_pDepthStencilView = nullptr;
	m_pSRV = nullptr;
	m_pTexture = nullptr;
	m_DSFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
}

void LDxRT::Set(FLOAT TopLeftX, FLOAT TopLeftY, FLOAT Width, FLOAT Height,
	FLOAT MinDepth, FLOAT MaxDepth)
{
	m_vp.TopLeftX = TopLeftX;
	m_vp.TopLeftY = TopLeftY;
	m_vp.Width = Width;
	m_vp.Height = Height;
	m_vp.MinDepth = MinDepth;
	m_vp.MaxDepth = MaxDepth;
	D3DXMatrixPerspectiveFovLH(&m_matProj, (float)XM_PI * 0.25f, Width / Height, 0.1f, 1000.0f);
}

HRESULT LDxRT::Create(FLOAT Width, FLOAT Height)
{
	HRESULT hr = S_OK;
	Set(0, 0, Width, Height, 0.0f, 1.0f);

	m_TexDesc.Width = (UINT)Width;
	m_TexDesc.Height = (UINT)Height;
	m_TexDesc.MipLevels = 1;
	m_TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_TexDesc.SampleDesc.Count = 1;
	m_TexDesc.SampleDesc.Quality = 0;
	m_TexDesc.Usage = D3D11_USAGE_DEFAULT;
	m_TexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	m_TexDesc.CPUAccessFlags = 0;
	m_TexDesc.MiscFlags = 0;
	m_TexDesc.ArraySize = 1;

	if (FAILED(hr = LGlobal::g_pDevice->CreateTexture2D(&m_TexDesc, NULL, m_pTexture.GetAddressOf())))
	{
		return hr;
	}
	if (FAILED(hr = LGlobal::g_pDevice->CreateShaderResourceView(m_pTexture.Get(), NULL, m_pSRV.GetAddressOf())))
	{
		return hr;
	}
	if (FAILED(hr = LGlobal::g_pDevice->CreateRenderTargetView(m_pTexture.Get(), NULL, m_pRenderTargetView.GetAddressOf())))
	{
		return hr;
	}


	// 백버퍼의 크기가 변경되면 반드시 깊이스텐실 뷰어도 다시 작성되어야 한다.		
	if (FAILED(hr = UpdateDepthStencilView((UINT)Width, (UINT)Height)))
	{
		return hr;
	}
	return hr;
}
HRESULT LDxRT::UpdateDepthStencilView(UINT Width, UINT Height)
{
	HRESULT hr;
	m_vp.Width = (FLOAT)Width;
	m_vp.Height = (FLOAT)Height;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDSTexture = nullptr;
	D3D11_TEXTURE2D_DESC DescDepth;
	ZeroMemory(&DescDepth, sizeof(D3D11_TEXTURE2D_DESC));
	DescDepth.Width = Width;
	DescDepth.Height = Height;
	DescDepth.MipLevels = 1;
	DescDepth.ArraySize = 1;
	if (m_DSFormat == DXGI_FORMAT_D24_UNORM_S8_UINT)
		DescDepth.Format = DXGI_FORMAT_R24G8_TYPELESS;
	else
		DescDepth.Format = DXGI_FORMAT_R32_TYPELESS;
	DescDepth.SampleDesc.Count = 1;
	DescDepth.SampleDesc.Quality = 0;
	DescDepth.CPUAccessFlags = 0;
	DescDepth.MiscFlags = 0;

	DescDepth.Usage = D3D11_USAGE_DEFAULT;
	// 백 버퍼 깊이 및 스텐실 버퍼 생성
	DescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	if (DescDepth.Format == DXGI_FORMAT_R24G8_TYPELESS || DescDepth.Format == DXGI_FORMAT_R32_TYPELESS)
	{
		// 깊이맵 전용 깊이맵 생성
		DescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	}


	/*DXGI_FORMAT texturefmt = DXGI_FORMAT_R32_TYPELESS;
	if(m_DSFormat == DXGI_FORMAT_D24_UNORM_S8_UINT)
		texturefmt = DXGI_FORMAT_R24G8_TYPELESS;
	else
		texturefmt = DXGI_FORMAT_R32_TYPELESS;*/

		//D3D11_TEXTURE2D_DESC dtd =
		//{
		//	Width,
		//	Height,
		//	1,//UINT MipLevels;
		//	1,//UINT ArraySize;
		//	texturefmt,//DXGI_FORMAT Format;
		//	1,//DXGI_SAMPLE_DESC SampleDesc;
		//	0,
		//	D3D11_USAGE_DEFAULT,//D3D11_USAGE Usage;
		//	D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE,//UINT BindFlags;
		//	0,//UINT CPUAccessFlags;
		//	0//UINT MiscFlags;    
		//};
	if (FAILED(hr = LGlobal::g_pDevice->CreateTexture2D(&DescDepth, NULL, pDSTexture.GetAddressOf())))
	{
		return hr;
	}

	///// 쉐이더 리소스 생성 : 깊이 맵 쉐도우에서 사용한다. ///
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	switch (DescDepth.Format)
	{
	case DXGI_FORMAT_R32_TYPELESS:
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		break;
	case DXGI_FORMAT_R24G8_TYPELESS:
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		break;
	}


	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	if (FAILED(hr = LGlobal::g_pDevice->CreateDepthStencilView(pDSTexture.Get(), &dsvDesc, m_pDepthStencilView.ReleaseAndGetAddressOf())))
	{
		return hr;
	}
	m_pDepthStencilView->GetDesc(&m_DepthStencilDesc);

	/*D3D11_SHADER_RESOURCE_VIEW_DESC dsrvd =
	{
		srvDesc.Format,
		D3D11_SRV_DIMENSION_TEXTURE2D,
		0,
		0
	};
	dsrvd.Texture2D.MipLevels = 1;*/

	if (srvDesc.Format == DXGI_FORMAT_R32_FLOAT || srvDesc.Format == DXGI_FORMAT_R24_UNORM_X8_TYPELESS)
	{
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		if (FAILED(hr = LGlobal::g_pDevice->CreateShaderResourceView(pDSTexture.Get(), &srvDesc, m_pDsvSRV.ReleaseAndGetAddressOf())))
		{
			return hr;
		}
	}
	return hr;
}
void LDxRT::Apply(ID3D11DeviceContext* pImmediateContext,
	ID3D11RenderTargetView* pRTV,
	ID3D11DepthStencilView* pDepthStencilView,
	bool bDepthClear, bool bStencilClear)
{
	assert(pImmediateContext);
	assert(pRTV);
	ID3D11RenderTargetView* pNullRTV = NULL;
	pImmediateContext->OMSetRenderTargets(1, &pNullRTV, NULL);

	if (pRTV != NULL)
	{
		if (pDepthStencilView != NULL)
			pImmediateContext->OMSetRenderTargets(1, &pRTV, pDepthStencilView);
		else
			pImmediateContext->OMSetRenderTargets(1, &pRTV, m_pDepthStencilView.Get());
	}
	else
	{
		if (pDepthStencilView != NULL)
			pImmediateContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), pDepthStencilView);
		else
			pImmediateContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	}
	if (bDepthClear)
	{
		if (pDepthStencilView != NULL)
			pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0, 0);
		else
			pImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0, 0);

	}
	if (bStencilClear)
	{
		if (pDepthStencilView != NULL)
			pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_STENCIL, 1.0, 0);
		else
			pImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_STENCIL, 1.0, 0);
	}
	pImmediateContext->RSSetViewports(1, &m_vp);
}
bool	LDxRT::Begin(TVector4 vColor,
	bool bTargetClear, bool bDepthClear, bool bStencilClear)
{
	m_nViewPorts = 1;
	LGlobal::g_pImmediateContext->RSGetViewports(&m_nViewPorts, m_vpOld);
	LGlobal::g_pImmediateContext->OMGetRenderTargets(1, &m_pOldRTV, &m_pOldDSV);

	ID3D11RenderTargetView* pNullRTV = NULL;
	LGlobal::g_pImmediateContext->OMSetRenderTargets(1, &pNullRTV, NULL);
	LGlobal::g_pImmediateContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

	Clear(vColor, bTargetClear, bDepthClear, bStencilClear);
	LGlobal::g_pImmediateContext->RSSetViewports(1, &m_vp);
	return true;
}
bool	LDxRT::Clear(TVector4 vColor,
	bool bTargetClear, bool bDepthClear, bool bStencilClear)
{
	if (bTargetClear)
	{
		const FLOAT color[] = { vColor.x, vColor.y, vColor.z, vColor.w };
		LGlobal::g_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
	}
	if (bDepthClear)
	{
		LGlobal::g_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0, 0);
	}
	if (bStencilClear)
	{
		LGlobal::g_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_STENCIL, 1.0, 0);
	}
	LGlobal::g_pImmediateContext->RSSetViewports(1, &m_vp);
	return true;
}
bool	LDxRT::End()
{
	LGlobal::g_pImmediateContext->RSSetViewports(m_nViewPorts, m_vpOld);
	LGlobal::g_pImmediateContext->OMSetRenderTargets(1, &m_pOldRTV, m_pOldDSV);
	LGlobal::g_pImmediateContext->PSSetShaderResources(0, 1, m_pSRV.GetAddressOf());
	if (m_pOldRTV) m_pOldRTV->Release();
	if (m_pOldDSV) m_pOldDSV->Release();
	return true;
}


