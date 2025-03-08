#pragma once
#include "MonoBehaviour.h"
class TextureList;
class DigitDisplay : public MonoBehaviour
{
public:
	DigitDisplay(int digit, wstring texPaths);
	~DigitDisplay();

	void Init() override;
	void Frame() override;
	void Render() override;
	void UpdateNumber(int num);
	void UpdateDigit(int digit);
	void LoadTextureList(wstring texXmlPath);
	wstring GetXmlPath() {return _texXmlPath;};
	void Save();

	int GetDigitNum() {return _digitNum;};
private:
	vector<shared_ptr<KObject>> _objects;
	wstring _texXmlPath;
	shared_ptr<TextureList> _texList;
	int _digitNum;

	int _num = 923; // 
};

