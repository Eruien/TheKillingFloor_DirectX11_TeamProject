#pragma once
#include "ResourceBase.h"
class TextureList : public ResourceBase
{
public:
	TextureList();
	~TextureList();
	void Save(wstring path);
	void Load(wstring path);
	void AddTexture(wstring path) { _texList.push_back(path); };
	void SetTexList(vector<wstring> paths) { _texList = paths; };
	void SetName(wstring name) { _name = name; }
	wstring GetName(){ return _name; }
	vector<wstring> GetTexList() { return _texList; };

public :
	vector<wstring> _texList;
	wstring _name;
};

