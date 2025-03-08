#pragma once
#include "LObject.h"

struct SkyBox_CB
{
	TMatrix matWorld;
	TMatrix matView;
	TMatrix matProj;
	TVector4 Color;
};

class LSkyBox : public LObject
{
public:
	ComPtr<ID3D11PixelShader> m_pPSCubeTexture = nullptr;
	ComPtr<ID3D11ShaderResourceView> m_pCubeTexSRV = nullptr;
	ComPtr<ID3D11SamplerState> m_pClampPointState = nullptr;
public:
	bool CreateVertexData() override;
	bool CreateIndexData() override;
	bool CreateConstantBuffer() override;
	bool Create(std::wstring shaderFileName, std::wstring texFileName) override;
	void CreateClampPoint();
	void SetMatrix(TMatrix* matWorld, TMatrix* matView, TMatrix* matProj) override;
public:
	bool Render() override;
};

