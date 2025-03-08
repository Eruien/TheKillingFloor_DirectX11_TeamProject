#pragma once
#include"LDXObject.h"

class LObject : public LDXObject
{
public:
	TMatrix m_matWorld;
	TMatrix m_matView;
	TMatrix m_matProj;
	TVector3 m_vScale;
	TVector3 m_vRotation;
	TVector3 m_vPosition;
public:
	virtual void SetMatrix(TMatrix* matWorld, TMatrix* matView, TMatrix* matProj);
	void  SetScale(TVector3 s);
	void  SetPos(TVector3 p);
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
public:
	LObject();
	virtual ~LObject();
};

