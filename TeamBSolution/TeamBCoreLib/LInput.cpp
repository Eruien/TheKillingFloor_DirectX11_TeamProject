#include "LInput.h"
#include "LGlobal.h"

INPUT_MAP g_InputData;


bool LInput::InitDirectInput()
{
    HRESULT hr = S_OK;
    if (FAILED(hr = DirectInput8Create(LGlobal::g_hInstance,
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        (void**)&m_pDI,
        NULL)))
    {
        return false;
    }
    if (FAILED(hr = m_pDI->CreateDevice(GUID_SysKeyboard,
        &m_pKeyDevice,
        NULL)))
    {
        return false;
    }
    // 장치별 반환데이터 설정
    m_pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
    if (FAILED(hr = m_pKeyDevice->SetCooperativeLevel(
        LGlobal::g_hWnd,
        DISCL_NONEXCLUSIVE |
        DISCL_FOREGROUND |
        DISCL_NOWINKEY)))
    {
        return false;
    }
    while (m_pKeyDevice->Acquire() == DIERR_INPUTLOST);

    if (FAILED(hr = m_pDI->CreateDevice(GUID_SysMouse,
        &m_pMouseDevice, NULL)))
    {
        return false;
    }
    m_pMouseDevice->SetDataFormat(&c_dfDIMouse);

    if (FAILED(hr = m_pMouseDevice->SetCooperativeLevel(
        LGlobal::g_hWnd,
        DISCL_NONEXCLUSIVE |
        DISCL_FOREGROUND)))
    {
        return false;
    }
    while (m_pMouseDevice->Acquire() == DIERR_INPUTLOST);
    return true;
}

bool LInput::ShutDownDirectInput()
{
    if (m_pKeyDevice)
    {
        m_pKeyDevice->Unacquire();
        m_pKeyDevice->Release();
        m_pKeyDevice = NULL;
    }
    if (m_pMouseDevice)
    {
        m_pMouseDevice->Unacquire();
        m_pMouseDevice->Release();
        m_pMouseDevice = NULL;
    }
    if (m_pDI)
    {
        m_pDI->Release();
        m_pDI = NULL;
    }
    return true;
}

BYTE LInput::GetKey(BYTE dwKey)
{
    BYTE sKey;
    sKey = m_KeyState[dwKey];
    if (sKey & 0x80)
    {
        if (m_KeyStateOld[dwKey] == KEY_FREE)
        {
            m_KeyStateOld[dwKey] = KEY_PUSH;
        }
        else
        {
            m_KeyStateOld[dwKey] = KEY_HOLD;
        }
    }
    else
    {
        if (m_KeyStateOld[dwKey] == KEY_PUSH
            || m_KeyStateOld[dwKey] == KEY_HOLD)
        {
            m_KeyStateOld[dwKey] = KEY_UP;
        }
        else
        {
            m_KeyStateOld[dwKey] = KEY_FREE;
        }
    }
    return m_KeyStateOld[dwKey];
}

TVector3 LInput::GetWorldPos(float windowWidth, float windowHeight, float cameraPosX, float cameraPosY)
{
    float HalfWindowX = windowWidth * 0.5f;
    float HalfWindowY = windowHeight * 0.5f;
    TVector3 mouseLTPos = { cameraPosX - HalfWindowX,
                          cameraPosY + HalfWindowY, 1.0f };
    mouseLTPos.x = mouseLTPos.x + m_MousePos.x;
    mouseLTPos.y = mouseLTPos.y - m_MousePos.y;

    return mouseLTPos;
}

bool LInput::Init()
{
    InitDirectInput();
	return true;
}

bool LInput::Frame()
{

    // 클라이언트 중앙 포지션 계산 
    RECT clientRect;
    GetClientRect(LGlobal::g_hWnd, &clientRect);

    int clientCenterX = (clientRect.right - clientRect.left) / 2;
    int clientCenterY = (clientRect.bottom - clientRect.top) / 2;
    POINT point = { clientCenterX, clientCenterY };
    ClientToScreen(LGlobal::g_hWnd, &point);

    // 화면 좌표
    ::GetCursorPos(&m_MousePos);
    // 클라이언트 좌표
    ::ScreenToClient(LGlobal::g_hWnd, &m_MousePos);

#pragma region 다이렉트 인풋 상태 읽어오기
    HRESULT hr;
    if (m_pMouseDevice == NULL || m_pKeyDevice == NULL) return false;
    
    if (FAILED(hr = m_pKeyDevice->GetDeviceState(256, m_KeyState)))
    {
        while (m_pKeyDevice->Acquire() == DIERR_INPUTLOST);
    }
    if (FAILED(hr = m_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_DIMouseState)))
    {
        while (m_pMouseDevice->Acquire() == DIERR_INPUTLOST);
    }
#pragma endregion

#pragma region 마우스 상태
    for (int iButton = 0; iButton < 3; iButton++)
    {
        m_BeforeMouseState[iButton] = m_DIMouseState.rgbButtons[iButton];
    }
    for (int iButton = 0; iButton < 3; iButton++)
    {
        if (m_BeforeMouseState[iButton] & 0x80)
        {
            if (m_MouseState[iButton] == KEY_FREE)
                m_MouseState[iButton] = KEY_PUSH;
            else
                m_MouseState[iButton] = KEY_HOLD;
        }
        else
        {
            if (m_MouseState[iButton] == KEY_PUSH ||
                m_MouseState[iButton] == KEY_HOLD)
                m_MouseState[iButton] = KEY_UP;
            else
                m_MouseState[iButton] = KEY_FREE;
        }
    }




    ZeroMemory(&g_InputData, sizeof(INPUT_MAP));

    if (m_MouseState[0] == KEY_PUSH)
        g_InputData.bLeftClick = true;
    if (m_MouseState[1] == KEY_PUSH) g_InputData.bRightClick = true;
    if (m_MouseState[2] == KEY_PUSH) g_InputData.bMiddleClick = true;

    if (m_MouseState[0] >= KEY_PUSH) g_InputData.bLeftHold = true;
    if (m_MouseState[1] >= KEY_PUSH) g_InputData.bRightHold = true;
    if (m_MouseState[2] >= KEY_PUSH) g_InputData.bMiddleHold = true;

    g_InputData.iMouseValue[0] = m_DIMouseState.lX;
    g_InputData.iMouseValue[1] = m_DIMouseState.lY;
    g_InputData.iMouseValue[2] = m_DIMouseState.lZ;
#pragma endregion


    g_InputData.bWKey = GetKey(DIK_W);
    g_InputData.bAKey = GetKey(DIK_A);
    g_InputData.bSKey = GetKey(DIK_S);
    g_InputData.bDKey = GetKey(DIK_D);
    g_InputData.bQKey = GetKey(DIK_Q);
    g_InputData.bEKey = GetKey(DIK_E);
    g_InputData.bEKey = GetKey(DIK_Z);
    g_InputData.bEKey = GetKey(DIK_B);
   
    g_InputData.bLShift = GetKey(DIK_LSHIFT);

    g_InputData.bLeftKey = GetKey(DIK_LEFT);
    g_InputData.bRightKey = GetKey(DIK_RIGHT);
    g_InputData.bUpKey = GetKey(DIK_UP);
    g_InputData.bDownKey = GetKey(DIK_DOWN);
    g_InputData.bEnter = GetKey(DIK_RETURN);
    g_InputData.bExit = GetKey(DIK_ESCAPE);
    GetKey(DIK_SPACE);
    g_InputData.bF1Key = GetKey(DIK_F1);
    GetKey(DIK_1);
    GetKey(DIK_2);
    GetKey(DIK_3);
    GetKey(DIK_4);
    GetKey(DIK_5);
    GetKey(DIK_6);
    GetKey(DIK_7);
    GetKey(DIK_F2);
    GetKey(DIK_F3);
    GetKey(DIK_F4);
    GetKey(DIK_R);
    GetKey(DIK_F);
    GetKey(DIK_F7);
    GetKey(DIK_F8);
    GetKey(DIK_X);
    
    if (GetKey(DIK_F5) == KEY_HOLD)
        g_InputData.bChangeFillMode = true;

    //return true; // 이 아래 부분은 12.23 이전 코드와의 호환을 위해 남겨놓음. 임시. [시진]

    m_vOffset.x = m_MousePos.x - m_BeforeMousePos.x;
    m_vOffset.y = m_MousePos.y - m_BeforeMousePos.y;

    if (ISEndPoint)
    {
        m_vOffset.x = 0.0f;
        m_vOffset.y = 0.0f;
        ISEndPoint = false;
    }

    //for (int ikey = 0; ikey < 256; ikey++)
    //{
    //    SHORT s = GetAsyncKeyState(ikey);
    //    if (s & 0x8000)
    //    {
    //        if (m_dwKeyState[ikey] == DWORD(KeyState::KEY_FREE))
    //        {
    //            m_dwKeyState[ikey] = DWORD(KeyState::KEY_PUSH);
    //        }
    //        else
    //        {
    //            m_dwKeyState[ikey] = DWORD(KeyState::KEY_HOLD);
    //        }
    //    }
    //    else
    //    {
    //        if (m_dwKeyState[ikey] == DWORD(KeyState::KEY_HOLD)
    //        ||  m_dwKeyState[ikey] == DWORD(KeyState::KEY_PUSH))
    //        {
    //            m_dwKeyState[ikey] = DWORD(KeyState::KEY_UP);
    //        }
    //        else
    //        {
    //            m_dwKeyState[ikey] = DWORD(KeyState::KEY_FREE);
    //        }
    //    }
    //}

    if (m_KeyStateOld[DIK_F2] == KEY_PUSH)
    {
        CursorChange();
    }

    if (IsHideCursor)
    {
        if (m_MousePos.x <= clientRect.left + m_MouseOffset || m_MousePos.x >= clientRect.right - m_MouseOffset ||
            m_MousePos.y <= clientRect.top + m_MouseOffset || m_MousePos.y >= clientRect.bottom - m_MouseOffset)
        {
            SetCursorPos(point.x, point.y);
            ISEndPoint = true;
        }
    }

    m_BeforeMousePos = m_MousePos;
	return true;
}

void LInput::CursorChange()
{
    ShowCursor(IsHideCursor);
    IsHideCursor = !IsHideCursor;
}


bool LInput::Render()
{
#ifdef _DEBUG
    /*std::wstring mousePos = std::to_wstring(m_MousePos.x);
    mousePos += L",";
    mousePos += std::to_wstring(m_MousePos.y);
    mousePos += L"\n";
    L_DebugString(mousePos.c_str());*/
#endif
	return true;
}

bool LInput::Release()
{
    ShutDownDirectInput();
	return true;
}