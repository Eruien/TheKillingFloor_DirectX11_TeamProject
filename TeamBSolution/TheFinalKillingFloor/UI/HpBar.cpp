#include "HpBar.h"
#include "KObject.h"
#include "LGlobal.h"
#include "../LPlayer.h"
HpBar::HpBar() : MonoBehaviour(L"HpBar")
{

}


HpBar::~HpBar()
{
}


void HpBar::Init()
{


}

void HpBar::Frame()
{

}

void HpBar::UpdateHp()
{
	float hp = LGlobal::g_PlayerModel->m_HP;


	GetGameObject()->m_VertexList[1].p.x = 0.5f - (1-hp / _maxHp);
	GetGameObject()->m_VertexList[4].p.x = 0.5f - (1-hp / _maxHp);
	GetGameObject()->m_VertexList[5].p.x = 0.5f - (1-hp / _maxHp);

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
