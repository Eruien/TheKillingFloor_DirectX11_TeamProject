#include "ChangeTexture.h"
#include "KObject.h"
#include "LGlobal.h"
#include "LManager.h"
ChangeTexture::ChangeTexture() : MonoBehaviour(L"ChangeTexture")
{
}

ChangeTexture::~ChangeTexture()
{

	
}

void ChangeTexture::Init()
{
}

void ChangeTexture::Frame()
{
	
}

void ChangeTexture::ChangeFromPath(wstring path)
{
	//LTexture* temp = LManager<LTexture>::GetInstance().Load(path);
	GetGameObject()->m_Tex = LManager<LTexture>::GetInstance().Load(path);
	GetGameObject()->m_Tex->Apply();
	//delete temp;

}

void ChangeTexture::ChangeFromManager(wstring key)
{

}
