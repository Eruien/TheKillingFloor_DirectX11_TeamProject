#pragma once
#include "LStd.h"
#include "LManager.h"
#include "LManagerAgr.h"



struct CB_Data
{
	TMatrix matWorld;
	TMatrix matView;
	TMatrix matProj;
};

class LDXObject
{
public:
	ComPtr<ID3D11Device> m_pDevice;
	ComPtr<ID3D11DeviceContext> m_pImmediateContext;
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;
	ComPtr<ID3D11Buffer> m_pConstantBuffer;
	ComPtr<ID3D11InputLayout> m_pVertexLayout;
	std::vector<SimpleVertex> m_VertexList;
	std::vector<DWORD> m_IndexList;
	LTexture* m_Tex = nullptr;
	LShader* m_Shader = nullptr;
	CB_Data m_cbData;
public:
	void Set();
	virtual bool CreateVertexData();
	virtual bool CreateIndexData();
	virtual bool CreateVertexBuffer();
	virtual bool CreateIndexBuffer();
	virtual bool CreateConstantBuffer();
	virtual bool CreateLayout();
	virtual bool Create(std::wstring shaderFileName, std::wstring texFileName);
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool PreRender();
	virtual bool Render();
	virtual bool PostRender();
	virtual bool Release();
public:
	virtual ~LDXObject() {}
};

