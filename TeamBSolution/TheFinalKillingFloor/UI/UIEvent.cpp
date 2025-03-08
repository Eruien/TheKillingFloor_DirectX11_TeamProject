#include "UIEvent.h"
#include "KObject.h"
#include "LGlobal.h"
#include "../LPlayer.h"
#include "ChangeTexture.h"
UIEvent::UIEvent(wstring Function) : MonoBehaviour(L"UIEvent")
{
	_function = Function;

	//프레임마다
	_functionMapFrame[L"HitPlayerEffect"] = &UIEvent::HitPlayerEffect;
	_functionMapFrame[L"RushEffect"] = &UIEvent::RushEffect;
	_functionMapFrame[L"SteamPack"] = &UIEvent::SteamPack;


	//한번
	_functionMap[L"UpdatePlayerFace"] = &UIEvent::UpdatePlayerFace;
}

UIEvent::~UIEvent()
{
}

void UIEvent::Init()
{
	//if(_function == L"HitPlayerEffect")
	//{
	//GetGameObject()->Create(L"../../res/hlsl/Fade.hlsl", L"../../res/ui/GoreSplash.png");
	//CreateConstantBuffer();
	//}
	if (_function == L"HitPlayerEffect")
	{
		GetGameObject()->Create(L"../../res/hlsl/Fade.hlsl", L"../../res/ui/GoreSplash.png");
		CreateConstantBuffer();
	}
	if (_function == L"RushEffect")
	{
		GetGameObject()->Create(L"../../res/hlsl/Fade.hlsl", L"../../res/ui/dash.png");
		CreateConstantBuffer();
	}
}

void UIEvent::Frame()
{
	if (_functionMapFrame.find(_function) != _functionMapFrame.end())
	{
			(this->*_functionMapFrame[_function])();
	}
	else {
		std::cout << "Function not found" << std::endl;
	}


}

void UIEvent::Render()
{


}

void UIEvent::HitPlayerEffect()
{
	if (GetGameObject()->GetIsRender())
	{
	
		_CTime += LGlobal::g_fSPF / _duration;
		data.alpha[0] = _CTime;

		LGlobal::g_pImmediateContext->UpdateSubresource(_cBuff.Get(), 0, NULL, &data, 0, 0);
		LGlobal::g_pImmediateContext->PSSetConstantBuffers(5, 1, _cBuff.GetAddressOf());
		if (data.alpha[0] >= 1.0f)
		{
			//data.alpha[0] = 0;
			_CTime = 0;
			GetGameObject()->SetIsRender(false);

		}
	}
}

void UIEvent::RushEffect()
{
	
	if (_cBuff == nullptr)
	{
		Init();
	}
	if (GetGameObject()->GetIsRender())
	{

		
		data.alpha[0] = _CTime;

		LGlobal::g_pImmediateContext->UpdateSubresource(_cBuff.Get(), 0, NULL, &data, 0, 0);
		LGlobal::g_pImmediateContext->PSSetConstantBuffers(5, 1, _cBuff.GetAddressOf());
		if (_CTime >= 1.f)
		{
			_CTime = 1.f; 
			_duration = -_duration; 
		}
		else if (_CTime <= 0.f)
		{
			_CTime = 0.f; 
			_duration = -_duration; 
		}

		_CTime += LGlobal::g_fSPF / (_duration*0.3);
	}

}

void UIEvent::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC bufferDesc{};
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.ByteWidth = sizeof(cb_DataA);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	HRESULT hr = LGlobal::g_pDevice->CreateBuffer(
		&bufferDesc,
		nullptr,
		_cBuff.GetAddressOf());

}

void UIEvent::SteamPack()
{
	
	float duration = LGlobal::g_PlayerModel->m_SteamPackStart;
	if (duration > 0.001f)
	{
		duration = LGlobal::g_PlayerModel->m_SteamPackEnd - duration;
		duration *= (100 / LGlobal::g_PlayerModel->m_SteamPackEnd);
	}
	

	GetGameObject()->m_VertexList[0].p.y = 0.5f - (1 - duration / 100);
	GetGameObject()->m_VertexList[1].p.y = 0.5f - (1 - duration / 100);
	GetGameObject()->m_VertexList[4].p.y = 0.5f - (1 - duration / 100);

	D3D11_BUFFER_DESC bufferDesc = { 0, };
	bufferDesc.ByteWidth = sizeof(SimpleVertex) * GetGameObject()->m_VertexList.size();
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initialData = { 0, };
	initialData.pSysMem = &GetGameObject()->m_VertexList.at(0);

	HRESULT hr = GetGameObject()->m_pDevice->CreateBuffer(
		&bufferDesc,
		&initialData,
		&GetGameObject()->m_pVertexBuffer);

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Create Buffer Error", "Error Box", MB_OK);
		return;
	}
}

void UIEvent::UpdatePlayerFace()
{
	 float hp = LGlobal::g_PlayerModel->m_HP;
	if (hp >= 61.0f)
	{
	GetGameObject()->GetScript<ChangeTexture>(L"ChangeTexture")->ChangeFromPath(L"../../res/ui/face.png");
	}
	else if (hp >= 21.0f)
	{
		GetGameObject()->GetScript<ChangeTexture>(L"ChangeTexture")->ChangeFromPath(L"../../res/ui/face2.png");
	}
	else
	{
		GetGameObject()->GetScript<ChangeTexture>(L"ChangeTexture")->ChangeFromPath(L"../../res/ui/face3.png");
	}
}
