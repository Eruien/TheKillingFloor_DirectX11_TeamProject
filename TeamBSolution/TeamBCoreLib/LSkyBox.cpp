#include "LSkyBox.h"
#include "LGlobal.h"
#include "LManager.h"
#include "LManagerAgr.h"

bool LSkyBox::CreateVertexData()
{
	// ¾Õ¸é
	m_VertexList.resize(24);
	m_VertexList[0] = SimpleVertex(TVector3(-1.0f, 1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(0.0f, 0.0f));
	m_VertexList[1] = SimpleVertex(TVector3(1.0f, 1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(1.0f, 0.0f));
	m_VertexList[2] = SimpleVertex(TVector3(1.0f, -1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(1.0f, 1.0f));
	m_VertexList[3] = SimpleVertex(TVector3(-1.0f, -1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(0.0f, 1.0f));
	// µÞ¸é
	m_VertexList[4] = SimpleVertex(TVector3(1.0f, 1.0f, 1.0f), TVector3(0.0f, 0.0f, 1.0f), TVector4(0.0f, 0.0f, 0.0f, 1.0f), TVector2(0.0f, 0.0f));
	m_VertexList[5] = SimpleVertex(TVector3(-1.0f, 1.0f, 1.0f), TVector3(0.0f, 0.0f, 1.0f), TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 0.0f));
	m_VertexList[6] = SimpleVertex(TVector3(-1.0f, -1.0f, 1.0f), TVector3(0.0f, 0.0f, 1.0f), TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 1.0f));
	m_VertexList[7] = SimpleVertex(TVector3(1.0f, -1.0f, 1.0f), TVector3(0.0f, 0.0f, 1.0f), TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(0.0f, 1.0f));

	// ¿À¸¥ÂÊ
	m_VertexList[8] = SimpleVertex(TVector3(1.0f, 1.0f, -1.0f), TVector3(1.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(0.0f, 0.0f));
	m_VertexList[9] = SimpleVertex(TVector3(1.0f, 1.0f, 1.0f), TVector3(1.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(1.0f, 0.0f));
	m_VertexList[10] = SimpleVertex(TVector3(1.0f, -1.0f, 1.0f), TVector3(1.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(1.0f, 1.0f));
	m_VertexList[11] = SimpleVertex(TVector3(1.0f, -1.0f, -1.0f), TVector3(1.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(0.0f, 1.0f));

	// ¿ÞÂÊ
	m_VertexList[12] = SimpleVertex(TVector3(-1.0f, 1.0f, 1.0f), TVector3(-1.0f, 0.0f, 0.0f), TVector4(1.0f, 1.0f, 0.0f, 1.0f), TVector2(0.0f, 0.0f));
	m_VertexList[13] = SimpleVertex(TVector3(-1.0f, 1.0f, -1.0f), TVector3(-1.0f, 0.0f, 0.0f), TVector4(1.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 0.0f));
	m_VertexList[14] = SimpleVertex(TVector3(-1.0f, -1.0f, -1.0f), TVector3(-1.0f, 0.0f, 0.0f), TVector4(1.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 1.0f));
	m_VertexList[15] = SimpleVertex(TVector3(-1.0f, -1.0f, 1.0f), TVector3(-1.0f, 0.0f, 0.0f), TVector4(1.0f, 1.0f, 0.0f, 1.0f), TVector2(0.0f, 1.0f));

	// À­¸é
	m_VertexList[16] = SimpleVertex(TVector3(-1.0f, 1.0f, 1.0f), TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 0.5f, 1.0f, 1.0f), TVector2(0.0f, 0.0f));
	m_VertexList[17] = SimpleVertex(TVector3(1.0f, 1.0f, 1.0f), TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 0.5f, 1.0f, 1.0f), TVector2(1.0f, 0.0f));
	m_VertexList[18] = SimpleVertex(TVector3(1.0f, 1.0f, -1.0f), TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 0.5f, 1.0f, 1.0f), TVector2(1.0f, 1.0f));
	m_VertexList[19] = SimpleVertex(TVector3(-1.0f, 1.0f, -1.0f), TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 0.5f, 1.0f, 1.0f), TVector2(0.0f, 1.0f));

	// ¾Æ·§¸é
	m_VertexList[20] = SimpleVertex(TVector3(-1.0f, -1.0f, -1.0f), TVector3(0.0f, -1.0f, 0.0f), TVector4(0.0f, 1.0f, 1.0f, 1.0f), TVector2(0.0f, 0.0f));
	m_VertexList[21] = SimpleVertex(TVector3(1.0f, -1.0f, -1.0f), TVector3(0.0f, -1.0f, 0.0f), TVector4(0.0f, 1.0f, 1.0f, 1.0f), TVector2(1.0f, 0.0f));
	m_VertexList[22] = SimpleVertex(TVector3(1.0f, -1.0f, 1.0f), TVector3(0.0f, -1.0f, 0.0f), TVector4(0.0f, 1.0f, 1.0f, 1.0f), TVector2(1.0f, 1.0f));
	m_VertexList[23] = SimpleVertex(TVector3(-1.0f, -1.0f, 1.0f), TVector3(0.0f, -1.0f, 0.0f), TVector4(0.0f, 1.0f, 1.0f, 1.0f), TVector2(0.0f, 1.0f));

	return true;
}

bool LSkyBox::CreateIndexData()
{
	m_IndexList.resize(36);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0; 	m_IndexList[iIndex++] = 1; 	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 0;	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 3;
	m_IndexList[iIndex++] = 4; 	m_IndexList[iIndex++] = 5; 	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 4;	m_IndexList[iIndex++] = 6; 	m_IndexList[iIndex++] = 7;
	m_IndexList[iIndex++] = 8; 	m_IndexList[iIndex++] = 9; 	m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 8;	m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 11;
	m_IndexList[iIndex++] = 12; m_IndexList[iIndex++] = 13; m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 12;	m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 15;
	m_IndexList[iIndex++] = 16; m_IndexList[iIndex++] = 17; m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 16;	m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 19;
	m_IndexList[iIndex++] = 20; m_IndexList[iIndex++] = 21; m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 20;	m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 23;

	return true;
}

bool LSkyBox::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC bufferDesc{};
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.ByteWidth = sizeof(SkyBox_CB);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	HRESULT hr = m_pDevice->CreateBuffer(
		&bufferDesc,
		nullptr,
		m_pConstantBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Create ConstantBuffer Error", "Error Box", MB_OK);
		return false;
	}

	return true;
}

bool LSkyBox::Create(std::wstring shaderFileName, std::wstring texFileName)
{
	LDXObject::Create(shaderFileName, texFileName);
	CreateClampPoint();
	m_Shader->LoadPixelShader(shaderFileName, "PS_CubeTexture");

	return true;
}

void LSkyBox::CreateClampPoint()
{
	D3D11_SAMPLER_DESC descSampler;
	ZeroMemory(&descSampler, sizeof(descSampler));
	descSampler.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

	descSampler.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	descSampler.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	descSampler.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	descSampler.MaxLOD = FLT_MAX;
	descSampler.MinLOD = FLT_MIN;

	/*descSampler.MipLODBias = 0;
	descSampler.MaxAnisotropy = 16;

	descSampler.ComparisonFunc = D3D11_COMPARISON_NEVER;

	descSampler.BorderColor[0] = 1.0f;
	descSampler.BorderColor[1] = 0.0f;
	descSampler.BorderColor[2] = 0.0f;
	descSampler.BorderColor[3] = 1.0f;*/

	LGlobal::g_pDevice->CreateSamplerState(&descSampler, m_pClampPointState.GetAddressOf());
}

void LSkyBox::SetMatrix(TMatrix* matWorld, TMatrix* matView, TMatrix* matProj)
{
	if (matWorld != nullptr)
	{
		m_matWorld = *matWorld;
	}

	if (matView != nullptr)
	{
		m_matView = *matView;
		m_matView._41 = 0;
		m_matView._42 = 0;
		m_matView._43 = 0;
	}

	if (matProj != nullptr)
	{
		m_matProj = *matProj;
	}
	
	m_cbData.matWorld = m_matWorld.Transpose();
	m_cbData.matView = m_matView.Transpose();
	m_cbData.matProj = m_matProj.Transpose();
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, nullptr, &m_cbData, 0, 0);

	return;
}

bool LSkyBox::Render()
{
	LDXObject::Render();

	LGlobal::g_pImmediateContext->PSSetSamplers(1, 1, m_pClampPointState.GetAddressOf());
	return true;
}