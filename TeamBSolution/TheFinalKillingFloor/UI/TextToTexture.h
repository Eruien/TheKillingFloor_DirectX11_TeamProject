#pragma once
#include "MonoBehaviour.h"
class TextureList;
class TextToTexture : public MonoBehaviour
{
public:
	TextToTexture(wstring text, wstring texPaths);
	~TextToTexture();

	void Init() override;
	void Frame() override;
	void Render() override;
	void UpdateText(wstring text);
	void SetText(wstring text) { _text = text; };
	void LoadTextureList(wstring texXmlPath);
	wstring GetXmlPath() { return _texXmlPath; };
	void Save();

	wstring GetText() { return _text; };
private:
	vector<shared_ptr<KObject>> _objects;
	wstring _texXmlPath;
	shared_ptr<TextureList> _texList;
	wstring _text;
	int _textLength;

};

