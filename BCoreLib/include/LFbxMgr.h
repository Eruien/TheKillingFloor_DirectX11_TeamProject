#pragma once
#include "LFbxImport.h"
#include "LFbxObj.h"

class LFbxMgr
{
public:
	static LFbxMgr& GetInstance()
	{
		static LFbxMgr input;
		return input;
	}
public:
	using LList = std::unordered_map<std::wstring, std::shared_ptr<LFbxObj>>;
	LList m_map;
	std::vector<LFbxObj*> m_ZombieMap;
	LFbxImport m_FbxImport;
public:
	LFbxObj* Load(std::wstring filePath, std::wstring shaderFilePath);
	LFbxObj* GetPtr(std::wstring key);
};

