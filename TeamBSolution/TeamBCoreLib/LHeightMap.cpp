#include "LHeightMap.h"

void LHeightMap::CreateHeightMap(std::wstring heightMapFilePath)
{
	HRESULT hr = DirectX::CreateWICTextureFromFileEx(m_pDevice.Get(),
		heightMapFilePath.c_str(),
		0,
		D3D11_USAGE_STAGING,
		0,
		D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ,
		0,
		DirectX::WIC_LOADER_DEFAULT,
		m_pTexture.GetAddressOf(), nullptr);

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "HeightMapTexture Load Error", "Error Box", MB_OK);
	}

	ID3D11Texture2D* pTexture2D = NULL;

	if (FAILED(m_pTexture->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTexture2D)))
	{
		MessageBoxA(NULL, "HeightMapTexture -> ID3D11Texture2D Error", "Error Box", MB_OK);
	}

	D3D11_TEXTURE2D_DESC desc;
	pTexture2D->GetDesc(&desc);

	m_fHeightList.resize(desc.Height * desc.Width);
	
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	hr = m_pImmediateContext->Map((ID3D11Resource*)pTexture2D, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_READ, 0, &MappedFaceDest);

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "ID3D11Resource Map Error", "Error Box", MB_OK);
	}

	UCHAR* pTexels = (UCHAR*)MappedFaceDest.pData;
	
	for (UINT row = 0; row < desc.Height; row++)
	{
		UINT rowStart = row * MappedFaceDest.RowPitch;
		for (UINT col = 0; col < desc.Width; col++)
		{
			UINT colStart = col * 4;
			UINT uRed = pTexels[rowStart + colStart + 0];
			m_fHeightList[row * desc.Width + col] = (float)uRed;	/// DWORDÀÌ¹Ç·Î pitch/4	
		}
	}

	m_pImmediateContext->Unmap(pTexture2D, D3D11CalcSubresource(0, 0, 1));

	if (pTexture2D) pTexture2D->Release();

	m_iNumRows = desc.Height;
	m_iNumCols = desc.Width;
}

float LHeightMap::GetHeightmap(int row, int col)
{
	return m_fHeightList[row * m_iNumRows + col] * m_MapDesc.fScaleHeight;
}

float LHeightMap::GetHeightOfVertex(int Index)
{
	return m_fHeightList[Index] * m_MapDesc.fScaleHeight;
};

TVector4 LHeightMap::GetColorOfVertex(int Index)
{
	return TVector4(1.0f, 1.0f, 1.0f, 1.0f);
};

