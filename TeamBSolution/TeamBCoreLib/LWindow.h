#pragma once
#include"LStd.h"

class LWindow
{
public:
    HINSTANCE m_hInstance;
    HWND m_hWnd;
    DWORD m_dwExStyle = WS_EX_APPWINDOW;
    DWORD m_dwStyle = WS_OVERLAPPEDWINDOW;
    int m_WindowWidth = 0;
    int m_WindowHeight = 0;
    int m_WindowPosX = 1920 / 2 - m_WindowWidth / 2;
    int m_WindowPosY = 1080 / 2 - m_WindowHeight / 2;
    RECT m_rcClient;
public:
    bool SetRegisterWindowClass(HINSTANCE hInstance);
    bool SetCreateWindow(LPCWSTR lpWindowName = L"�⺻â", int WindowWidth = 400, int WindowHeight = 300);
    int MsgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    virtual void ResizeDevice(UINT width, UINT height) {};
public:
    LWindow();
    virtual ~LWindow();
};

