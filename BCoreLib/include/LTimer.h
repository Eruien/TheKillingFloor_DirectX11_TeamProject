#pragma once
#include "LWrite.h"
#include <windows.h>
#pragma comment(lib, "winmm.lib")

class LTimer
{
public:
	float m_fGameTimer;
	float m_RegenTimer;
	DWORD m_dwBeforeTime;
	float m_fFPS;
	float m_fSPF;
	int m_iFPS;
public:
	bool RegenTime(float time);
	bool Init();
	float GetFPS();
	bool Frame();
	bool Render();
	bool Release();
};

