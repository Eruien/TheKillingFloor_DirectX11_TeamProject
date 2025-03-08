#include "TextToTexture.h"
#include "KObject.h"
#include "LGlobal.h"
#include "UICamera.h"
#include "TextureList.h"
TextToTexture::TextToTexture(wstring text, wstring texXmlPath) : _text(text), _texXmlPath(texXmlPath), MonoBehaviour(L"TextToTexture")
{
}

TextToTexture::~TextToTexture()
{
}

void TextToTexture::Init()
{

	UpdateText(_text);
}

void TextToTexture::Frame()
{
	for (int i = 0; i < _textLength; i++)
	{

		TVector3 pos = GetGameObject()->m_vPosition;
		pos.x = GetGameObject()->m_vPosition.x + (GetGameObject()->m_vScale.x / _textLength ) - (i * (GetGameObject()->m_vScale.x / _textLength ));
		_objects[i]->SetPos(pos);
		_objects[i]->SetScale({ GetGameObject()->m_vScale.x / _textLength ,GetGameObject()->m_vScale.y,0 });
		_objects[i]->SetMatrix(nullptr, &LGlobal::g_pUICamera->m_matView, &LGlobal::g_pUICamera->m_matOrthoProjection);
		_objects[i]->Frame();

	}
}

void TextToTexture::Render()
{
	for (auto obj : _objects)
	{
		obj->Render();
	}
}

void TextToTexture::UpdateText(wstring text)
{
	_text = text;
	_textLength = _text.size();
	
	LoadTextureList(_texXmlPath);
	GetGameObject()->m_Tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/Empty.png");
	GetGameObject()->m_Tex->Apply();

	_objects.clear();


	for (int i = 0; i < _textLength; i++)
	{
		_objects.push_back(make_shared<KObject>());
		_objects[i]->Init();
		_objects[i]->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/ui.Empty.png");
		TVector3 pos = GetGameObject()->m_vPosition;
		pos.x = GetGameObject()->m_vPosition.x + (GetGameObject()->m_vScale.x / _textLength) - (i * (GetGameObject()->m_vScale.x / _textLength));
		_objects[i]->SetPos(pos);
		_objects[i]->SetScale({ GetGameObject()->m_vScale.x / _textLength ,GetGameObject()->m_vScale.y,0 });
	}
	for (int i = 0; i < _textLength; ++i)
	{
		wchar_t character = _text[i];
		int index = -1;

		if (character >= 33 && character <= 126)
		{
			index = character - 33;
		}

		if (index != -1)
		{
			_objects[_textLength - 1 - i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList->GetTexList()[index]);
			_objects[_textLength - 1 - i]->m_Tex->Apply();
		}
	}
	//for (int i = _textLength - 1; i >= 0; --i)
	//{
	//	wchar_t character = _text[i];

	//	if (65 + i == character)
	//	{
	//		_objects[_textLength - 1 - i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList->GetTexList()[0]);
	//		_objects[_textLength - 1 - i]->m_Tex->Apply();
	//	}
	//	}

	/*for (int i = _textLength - 1; i >= 0; --i)
	{
		wchar_t character = _text[i];

		switch (character)
		{
		case 65:
			_objects[_textLength - 1 - i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList->GetTexList()[0]);
			_objects[_textLength - 1 - i]->m_Tex->Apply();
			break;
		case 66:
			_objects[_textLength - 1 - i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList->GetTexList()[1]);
			_objects[_textLength - 1 - i]->m_Tex->Apply();
			break;
		case 67:
			_objects[_textLength - 1 - i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList->GetTexList()[2]);
			_objects[_textLength - 1 - i]->m_Tex->Apply();
			break;
		case 68:
			_objects[_textLength - 1 - i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList->GetTexList()[3]);
			_objects[_textLength - 1 - i]->m_Tex->Apply();
			break;
		default:
			break;
		}
	

	}*/

}

void TextToTexture::LoadTextureList(wstring texXmlPath)
{
	_texList = make_shared<TextureList>();
	_texList->Load(texXmlPath);
}

void TextToTexture::Save()
{
}
