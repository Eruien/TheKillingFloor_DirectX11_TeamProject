#pragma once
#include "MonoBehaviour.h"
class Text : public MonoBehaviour
{
public:
	Text(wstring path);
	~Text();
	void Frame() override;
	wstring GetText() { return _text; };
	void SetText(wstring text) { _text = text; };
	void Load(wstring path);

private:
	wstring _text = L"NULL";
	wstring _textPath;
};

