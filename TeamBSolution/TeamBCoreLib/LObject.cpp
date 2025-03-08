#include "LObject.h"

void  LObject::SetMatrix(TMatrix* matWorld, TMatrix* matView, TMatrix* matProj)
{
	if (matWorld != nullptr)
	{
		m_matWorld = *matWorld;
	}

	if (matView != nullptr)
	{
		m_matView = *matView;
	}

	if (matProj != nullptr)
	{
		m_matProj = *matProj;
	}

	m_cbData.matWorld = m_matWorld.Transpose();
	m_cbData.matView = m_matView.Transpose();
	m_cbData.matProj = m_matProj.Transpose();
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, nullptr, &m_cbData, 0, 0);
}

void LObject::SetScale(TVector3 s)
{
	m_vScale = s;
}

void LObject::SetPos(TVector3 p)
{
	m_vPosition = p;
}

bool LObject::Init()
{	
	LDXObject::Init();
	return true;
}

bool LObject::Frame()
{
	LDXObject::Frame();
	return true;
}

bool LObject::Render()
{
	LDXObject::Render();
	return true;
}

bool LObject::Release()
{
	LDXObject::Release();
	return true;
}

LObject::LObject()
{
	m_vPosition = TVector3(0, 0, 0);
	m_vRotation = TVector3(0, 0, 0);
	m_vScale = TVector3(1, 1, 1);
}

LObject::~LObject() {}
