#include "UIEvent.h"
#include "KObject.h"
#include "LGlobal.h"
UIEvent::UIEvent(wstring Function) : MonoBehaviour(L"UIEvent")
{
	_functionMap[L"HitPlayerEffect"] = &UIEvent::HitPlayerEffect;
	data.alpha[0] = 1;
	CreateConstantBuffer();
}

UIEvent::~UIEvent()
{
}

void UIEvent::Frame()
{
}

void UIEvent::HitPlayerEffect()
{

	LGlobal::g_pImmediateContext->UpdateSubresource(_cBuff.Get(), 0, NULL, &data, 0, 0);
	LGlobal::g_pImmediateContext->VSSetConstantBuffers(2, 1, _cBuff.GetAddressOf());
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
