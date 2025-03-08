#pragma once
#include "LStd.h"
#include <windows.h>
#include <string>
#include <vector>
#include <dwrite.h>
#include <d2d1.h>
#include <d2d1helper.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

struct LTextData
{
	std::wstring text = L"";
	D2D1_RECT_F layout = { 0.0f, 0.0f, 800.0f, 600.0f };
	D2D1::ColorF color = { 0.0, 0.0f, 0.0f, 1.0f };
};

class LWrite
{
public:
	static LWrite& GetInstance()
	{
		static LWrite input;
		return input;
	}
public:
	std::vector<LTextData> m_TextList;
public:
	ComPtr<ID2D1Factory> m_pFactory;
	ComPtr<IDWriteFactory> m_pWriteFactory;
	ComPtr<IDWriteTextFormat> m_ptextFormat;
	ComPtr<IDXGISwapChain> m_pSwapChain;
	IDXGISurface1* m_pbackBuffer = nullptr;
	ComPtr<ID2D1RenderTarget> m_pRenderTarget;
	ComPtr<ID2D1SolidColorBrush> m_pSolidColorBrush;
public:
	void SetCreateFactory();
	void SetCreateWriteFactory();
	void SetCreateTextFormat();
	void SetCreateBackBuffer(IDXGISwapChain* pSwapChain);
	void SetCreateWriteRT();
	void SetCreateWriteRT(IDXGISurface1* pBackBuffer);
public:
	void AddText(std::wstring text, float x, float y, D2D1::ColorF color = { 0.0f, 0.0f, 0.0f, 1.0f });
public:
	bool Create(IDXGISwapChain* pSwapChain);
	bool Init();
	bool Frame();
	bool PreRender();
	bool Render();
	bool PostRender();
	bool Release();
public:
	virtual bool DeleteDxResource();
	virtual bool CreateDxResource(IDXGISurface1* pBackBuffer);
};

