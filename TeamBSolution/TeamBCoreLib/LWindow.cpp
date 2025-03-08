#include "LWindow.h"
#include "LGlobal.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT hr = LGlobal::g_pWindow->MsgProc(hWnd, message, wParam, lParam);
    if (SUCCEEDED(hr)) return 0;

    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

int LWindow::MsgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SIZE:
        if (SIZE_MINIMIZED != wParam)
        {
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            ResizeDevice(width, height);
        }
        break;
    }

    return -1;
}

bool LWindow::SetRegisterWindowClass(HINSTANCE hInstance)
{
    m_hInstance = hInstance;

    WNDCLASSEXW wcex = { 0, };

    wcex.cbSize = sizeof(WNDCLASSEXW);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(128, 128, 128));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.lpszClassName = L"LWindow";

    WORD ret = RegisterClassExW(&wcex);

    if (!ret)
    {
        MessageBoxA(NULL, "Window Class Register Error", "Error Box", MB_OK);
    }

    return true;
}

bool LWindow::SetCreateWindow(LPCWSTR lpWindowName, int WindowWidth, int WindowHeight)
{
    RECT rc = { 0,0, WindowWidth, WindowHeight };

    AdjustWindowRect(&rc, m_dwStyle, FALSE);

    m_hWnd = CreateWindowExW(
        m_dwExStyle,
        L"LWindow",
        lpWindowName,
        m_dwStyle,
        m_WindowPosX,
        m_WindowPosY,
        rc.right - rc.left,
        rc.bottom - rc.top,
        nullptr,
        nullptr,
        m_hInstance,
        nullptr);

    if (!m_hWnd)
    {
        MessageBoxA(NULL, "Create Window Error", "Error Box", MB_OK);
    }

    LGlobal::g_hWnd = m_hWnd;

    GetClientRect(m_hWnd, &m_rcClient);
    LGlobal::g_WindowWidth = m_WindowWidth = m_rcClient.right;
    LGlobal::g_WindowHeight = m_WindowHeight = m_rcClient.bottom;
    int x = 100;
    int y = 100;
    SetWindowPos(m_hWnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    ShowWindow(m_hWnd, SW_SHOW);

    return true;
}

LWindow::LWindow()
{
    LGlobal::g_pWindow = this;
}

LWindow::~LWindow() {}