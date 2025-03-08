#pragma once
#include "LStd.h"
#include "fmod.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#include "LManager.h"
#pragma comment(lib, "fmod_vc.lib")

class LSound
{
public:
	std::wstring m_csName = L"";
	std::wstring m_csPath = L"";
	float m_volume;
public:
	FMOD::System* m_pSystem = nullptr;
	FMOD::Sound* m_pSound = nullptr;
	FMOD::Channel* m_pChannel = nullptr;

	std::vector<FMOD::Channel*> m_channels;
public:
	void Set(FMOD::System* system);
	void SetName(std::wstring key);
	bool Load(std::wstring filePath);
public:
	void Play(bool bLoop = true);
	void PlayEffect();
	void Stop();
	void Pause();
	void VolumeUp();
	void VolumeDown();
	void SetVolume(float volume);
	void ToggleSound(bool toggle);
public:
	bool Init();
	bool Frame();
	bool Release();
};

class LSoundMgr
{
	FMOD::System* m_pSystem;
	using LList = std::map< std::wstring, std::unique_ptr<LSound>>;
public:
	static LSoundMgr& GetInstance()
	{
		static LSoundMgr input;
		return input;
	}
	LList   m_map;
public:
	LSound* Load(std::wstring filePath);
	LSound* GetPtr(std::wstring key);
	bool  Get(std::wstring key, LSound& ret);
public:
	bool  Frame();
	bool  Release();
private:
	LSoundMgr();
public:
	virtual ~LSoundMgr();
};