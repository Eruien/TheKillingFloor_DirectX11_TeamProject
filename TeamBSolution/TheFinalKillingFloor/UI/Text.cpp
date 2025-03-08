#include "Text.h"
#include "KObject.h"
#include "LWrite.h"
#include "LGlobal.h"
#include "tinyxml2.h"
Text::Text(wstring text) : MonoBehaviour(L"Text")
{
	_text = text;
}

Text::~Text()
{
}

void Text::Frame()
{
	LWrite::GetInstance().AddText(L"¾¾ºÎ¶ö", 1274, 714, {1,1,1,1});
	if(GetGameObject()->GetIsRender())
	LWRITE.AddText(_text, GetGameObject()->m_vPosition.x + LGlobal::g_WindowWidth / 2, -(GetGameObject()->m_vPosition.y - LGlobal::g_WindowHeight / 2), { 1,1,1,1 });
}


