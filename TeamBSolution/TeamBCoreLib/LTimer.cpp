#include "LTimer.h"
#include "LGlobal.h"

bool LTimer::Init()
{
	m_fGameTimer = 0.0f;
	m_dwBeforeTime = timeGetTime();
	m_RegenTimer = 0.0f;
	m_fFPS = 0.0f;
	m_fSPF = 0.0f;
	m_iFPS = 0.0f;

	return true;
}

float LTimer::GetFPS()
{
	static int iFPS = 0;

	if (m_fFPS >= 1.0f)
	{
		m_iFPS = iFPS;
		iFPS = 0;
		m_fFPS -= 1.0f;
	}

	++iFPS;

	return m_iFPS;
}

bool LTimer::RegenTime(float time)
{
	if (m_RegenTimer >= time)
	{
		m_RegenTimer = 0.0f;

		return true;
	}

	return false;
}

bool LTimer::Frame()
{
	DWORD dwCurrentTime = timeGetTime();
	float fElapsedTime = (dwCurrentTime - m_dwBeforeTime) / 1000.0f;
	LGlobal::g_fSPF = m_fSPF = fElapsedTime;
	m_fFPS += fElapsedTime;
	LGlobal::g_fGameTimer = m_fGameTimer += fElapsedTime;
	m_RegenTimer += fElapsedTime;
	m_dwBeforeTime = dwCurrentTime;

	return true;
}

bool LTimer::Render()
{
	std::wstring text = L"[GT]";
	text += std::to_wstring(m_fGameTimer);
	text += L"[FPS]"; 
	text += std::to_wstring(GetFPS());
	text += L"[SPF]";
	text += std::to_wstring(m_fSPF);
	
	//LWrite::GetInstance().AddText(text, 0.0f, 0.0f, {1.0f, 1.0f, 1.0f, 1.0f});

	return true;
}

bool LTimer::Release()
{
	return true;
}


