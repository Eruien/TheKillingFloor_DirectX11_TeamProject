#pragma once
#include "LStd.h"
#include "LCamera.h"
#include "UICamera.h"
#include "LSoundMgr.h"

class LWindow;
class LModel;
class LSkinningModel;
class LPlayer;

class LGlobal
{
public:
	static HWND g_hWnd;
	static HINSTANCE g_hInstance;
	static int g_WindowWidth;
	static int g_WindowHeight;
	static LWindow* g_pWindow;
	static ComPtr<ID3D11Device> g_pDevice;
	static ComPtr<ID3D11DeviceContext> g_pImmediateContext;
	static ComPtr<IDXGISwapChain> g_pSwapChain;
	static ComPtr<ID3D11DepthStencilState> g_pDepthStencilState;
	static ComPtr<ID3D11DepthStencilState> g_pDepthStencilStateDisable;
	static ComPtr<ID3D11Buffer> g_pSOBuffer;

	static float g_fSPF;
	static float g_fGameTimer;
	static float g_LoadPercent;
	static int g_IsLoding;
	static LCamera* g_pMainCamera;
	static UICamera* g_pUICamera;
	static LPlayer* g_PlayerModel;
};


