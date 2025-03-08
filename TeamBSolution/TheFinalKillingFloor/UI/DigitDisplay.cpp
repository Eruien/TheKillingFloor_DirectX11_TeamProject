#include "DigitDisplay.h"
#include "KObject.h"
#include "LGlobal.h"
#include "LCamera.h"
#include "TextureList.h"
DigitDisplay::DigitDisplay(int digit, wstring texXmlPath) : _digitNum(digit), _texXmlPath(texXmlPath), MonoBehaviour(L"DigitDisplay")
{
    
}

DigitDisplay::~DigitDisplay()
{
}

void DigitDisplay::Init()
{
    UpdateDigit(_digitNum);

}

void DigitDisplay::Frame()
{
    //TEST
    //UpdateNumber(123);


	for (int i = 0; i < _digitNum; i++)
	{

		TVector3 pos = GetGameObject()->m_vPosition;
		pos.x = GetGameObject()->m_vPosition.x + (GetGameObject()->m_vScale.x / _digitNum) - (i * (GetGameObject()->m_vScale.x / _digitNum));
		_objects[i]->SetPos(pos);
		_objects[i]->SetScale({ GetGameObject()->m_vScale.x / _digitNum,GetGameObject()->m_vScale.y,0 });
		_objects[i]->SetMatrix(nullptr, &LGlobal::g_pUICamera->m_matView, &LGlobal::g_pUICamera->m_matOrthoProjection);
		_objects[i]->Frame();

	}

}

void DigitDisplay::Render()
{
    for (auto& obj : _objects)
    {
        obj->Render();
    }
}

void DigitDisplay::UpdateNumber(int num)
{
    vector<int> digits;
    //int temp = _num;
 
    _num = num;
    int temp = _num;
    while (temp > 0) {
        int digit = temp % 10;
        digits.push_back(digit);
        temp /= 10;
    }

    if (_digitNum < digits.size())
    {
        UpdateDigit(digits.size());
    }
 /*   if (_digitNum < digits.size())
    {
        for (int i = digits.size() - _digitNum; i > 0 ; i--)
        {
            digits.push_back(0);
        }
        _digitNum = digits.size();
    }*/
    for (int i = 0; i < _digitNum; i++)
    {
        _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList->GetTexList()[0]);
        _objects[i]->m_Tex->Apply();
    }
    for (int i = 0; i < digits.size(); i++)
    {

        switch (digits[i]) {
        case 0:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList->GetTexList()[0]);
            _objects[i]->m_Tex->Apply();
            break;
        case 1:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList->GetTexList()[1]);
            _objects[i]->m_Tex->Apply();
            break;
        case 2:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList->GetTexList()[2]);
            _objects[i]->m_Tex->Apply();
            break;
        case 3:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList->GetTexList()[3]);
            _objects[i]->m_Tex->Apply();
            break;
        case 4:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList->GetTexList()[4]);
            _objects[i]->m_Tex->Apply();
            break;
        case 5:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList->GetTexList()[5]);
            _objects[i]->m_Tex->Apply();
            break;
        case 6:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList->GetTexList()[6]);
            _objects[i]->m_Tex->Apply();
            break;
        case 7:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList->GetTexList()[7]);
            _objects[i]->m_Tex->Apply();
            break;
        case 8:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList->GetTexList()[8]);
            _objects[i]->m_Tex->Apply();
            break;
        case 9:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(_texList->GetTexList()[9]);
            _objects[i]->m_Tex->Apply();
            break;
        default:
            std::cout << "Invalid value" << std::endl;
            break;
        }



    }
}

void DigitDisplay::UpdateDigit(int digit)
{
    _digitNum = digit;

    LoadTextureList(_texXmlPath);
    GetGameObject()->m_Tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/Empty.png");
    GetGameObject()->m_Tex->Apply();
    _objects.clear();
    for (int i = 0; i < _digitNum; i++)
    {
        _objects.push_back(make_shared<KObject>());
        _objects[i]->Init();
        _objects[i]->Create(L"../../res/hlsl/CustomizeMap.hlsl", _texList->GetTexList()[0]);
        TVector3 pos = GetGameObject()->m_vPosition;
        pos.x = GetGameObject()->m_vPosition.x + (GetGameObject()->m_vScale.x / _digitNum) - (i * (GetGameObject()->m_vScale.x / _digitNum));
        _objects[i]->SetPos(pos);
        _objects[i]->SetScale({ GetGameObject()->m_vScale.x / _digitNum,GetGameObject()->m_vScale.y,0 });
    }

}

void DigitDisplay::LoadTextureList(wstring texXmlPath)
{
    _texList = make_shared<TextureList>();
    _texList->Load(texXmlPath);
}


void DigitDisplay::Save()
{
    
}
