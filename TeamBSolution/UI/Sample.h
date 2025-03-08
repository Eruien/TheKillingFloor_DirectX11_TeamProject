#pragma once
#include "LCore.h"
#include "UICamera.h"
#include "UIManager.h"
#include "KObject.h"




class Sample : public LCore
{
public:
	std::shared_ptr<UICamera> m_DebugCamera = nullptr;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	shared_ptr<KObject> obj;
	vector<shared_ptr<KObject>> _objects;
	shared_ptr<KObject> sObj;
	shared_ptr<KObject> sObj2;



	

public:
	virtual ~Sample();
};

