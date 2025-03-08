#define DIRECTINPUT_VERSION 0x0800


#pragma once
#include "LStd.h"
// direct input
#include "dinput.h"
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

enum KeyState {
	KEY_FREE,
	KEY_PUSH,
	KEY_HOLD,
	KEY_UP
};

//enum class KeyState
//{
//	KEY_FREE = 0,
//	KEY_UP = 1,
//	KEY_PUSH = 2,
//	KEY_HOLD = 3,
//};

struct INPUT_MAP
{
	bool bUpKey;
	bool bDownKey;
	bool bLeftKey;
	bool bRightKey;

	bool bWKey;
	bool bSKey;
	bool bAKey;
	bool bDKey;
	bool bQKey;
	bool bZKey;
	bool bCKey;
	bool bEKey;

	bool bLeftClick;
	bool bRightClick;
	bool bMiddleClick;

	bool bLeftHold;
	bool bRightHold;
	bool bMiddleHold;

	bool bEnter;
	bool bExit;
	bool bSpace;
	
	int iMouseValue[3];

	bool bFullScreen;
	bool bChangeFillMode;
	bool bChangePrimitive;
	bool bChangeCullMode;
	bool bChangeCameraType;
	bool bDebugRender;
	bool bF1Key;

	bool bLShift;
};
extern INPUT_MAP g_InputData; // 전역 구조체로 마우스 상태와 키 상태를 전역으로 저장.


class LInput
{
public:
	static LInput& GetInstance()
	{
		static LInput input;
		return input;
	}

public: // for direct input , [시진]
	IDirectInput8* m_pDI;
	IDirectInputDevice8* m_pKeyDevice;  // 키보드
	IDirectInputDevice8* m_pMouseDevice;// 마우스
	DIMOUSESTATE	m_DIMouseState;
	BYTE	m_MouseState[3];
	BYTE	m_BeforeMouseState[3];
	KeyState m_KeyStateOld[256];
public:
	bool m_StartClick = false;
	bool IsHideCursor = false;
	bool ISEndPoint = false;
	float m_MouseOffset = 10.0f;
private:
	BYTE	m_KeyState[256];
	BYTE GetKey(BYTE dwKey);
	

public:
	bool InitDirectInput();
	bool ShutDownDirectInput();
	void CursorChange();
public:
	TVector3 m_vOffset = { 0,0,0 };
	POINT m_MousePos;
	POINT m_BeforeMousePos;
	//DWORD m_dwKeyState[256] = {0,};

	POINT GetMousePos() {
		return m_MousePos;
	}
	
public:
	TVector3 GetWorldPos(float windowWidth, float windowHeight, float cameraPosX, float cameraPosY);
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
private:
	LInput() {}
public:
	~LInput() {}
};

