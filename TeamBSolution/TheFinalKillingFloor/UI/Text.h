#pragma once
#include "MonoBehaviour.h"
class Text : public MonoBehaviour
{
public:
	Text(wstring text);
	~Text();
	void Frame() override;
	wstring GetText() { return _text; };
	void SetText(wstring text) { _text = text; };

private:
	wstring _text = L"NULL";
};

