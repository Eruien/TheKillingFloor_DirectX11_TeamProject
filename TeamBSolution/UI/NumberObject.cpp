#include "NumberObject.h"
#include "LGlobal.h"
#include "LInput.h"
bool NumberObject::Init()
{
	KObject::Init();

	for (int i = 0; i < _digitNum; i++)
	{
		_objects.push_back(make_shared<KObject>());
		_objects[i]->Init();
		_objects[i]->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/ui/0.png");
		TVector3 pos = m_vPosition;
		pos.x = m_vPosition.x + (m_vScale.x / _digitNum)-(i* (m_vScale.x / _digitNum));
		_objects[i]->SetPos(pos);
		_objects[i]->SetScale({m_vScale.x/ _digitNum,m_vScale.y,0});
	}
	return true;
}

bool NumberObject::Frame()
{
	KObject::Frame();
	for (int i = 0; i < _digitNum; i++)
	{
		
		TVector3 pos = m_vPosition;
        pos.x = m_vPosition.x + (m_vScale.x / _digitNum) * (_digitNum - 1) / 2 - (i * (m_vScale.x / _digitNum));
		_objects[i]->SetPos(pos);
		_objects[i]->SetScale({ m_vScale.x / _digitNum,m_vScale.y,0 });
		_objects[i]->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
		_objects[i]->Frame();
		
	}

	return true;
}

bool NumberObject::Render()
{
	//KObject::Render();
	for (auto obj : _objects)
	{
		obj->Render();
	}
	return true;
}
//GetGameObject()->m_Tex->m_pTexSRV = LManager<LTexture>::GetInstance().Load(path)->m_pTexSRV;
//GetGameObject()->m_Tex->Apply();
void NumberObject::UpdateNumber()
{
    vector<int> digits;
    //int temp = _num;

    int temp = LGlobal::g_fGameTimer;

    while (temp > 0) {
        int digit = temp % 10;
        digits.push_back(digit);
        temp /= 10;
    }
  
    for (int i = 0; i < _digitNum; i++)
    {
        _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/0.png");
        _objects[i]->m_Tex->Apply();
    }
    for (int i = 0; i < digits.size(); i++)
    {
        
        switch (digits[i]) {
        case 0:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/0.png");
            _objects[i]->m_Tex->Apply();
            break;
        case 1:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/1.png");
            _objects[i]->m_Tex->Apply();
            break;
        case 2:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/2.png");
            _objects[i]->m_Tex->Apply();
            break;
        case 3:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/3.png");
            _objects[i]->m_Tex->Apply();
            break;
        case 4:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/4.png");
            _objects[i]->m_Tex->Apply();
            break;
        case 5:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/5.png");
            _objects[i]->m_Tex->Apply();
            break;
        case 6:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/6.png");
            _objects[i]->m_Tex->Apply();
            break;
        case 7:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/7.png");
            _objects[i]->m_Tex->Apply();
            break;
        case 8:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/8.png");
            _objects[i]->m_Tex->Apply();
            break;
        case 9:
            _objects[i]->m_Tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/9.png");
            _objects[i]->m_Tex->Apply();
            break;
        default:
            std::cout << "Invalid value" << std::endl;
            break;
        }



    }


}
