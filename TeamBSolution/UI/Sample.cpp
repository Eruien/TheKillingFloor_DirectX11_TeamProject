#include "Sample.h"
#include "LGlobal.h"



bool Sample::Init()
{
	UIManager::GetInstance().Init();
	UIManager::GetInstance().Load(L"testScene.xml");
	//TextureList* tex = new TextureList();
	//tex->SetName(L"DefaultNumber");
	//tex->
	// ure(L"../../res/ui/0.png");
	//tex->AddTexture(L"../../res/ui/1.png");
	//tex->AddTexture(L"../../res/ui/2.png");
	//tex->AddTexture(L"../../res/ui/3.png");
	//tex->AddTexture(L"../../res/ui/4.png");
	//tex->AddTexture(L"../../res/ui/5.png");
	//tex->AddTexture(L"../../res/ui/6.png");
	//tex->AddTexture(L"../../res/ui/7.png");
	//tex->AddTexture(L"../../res/ui/8.png");
	//tex->AddTexture(L"../../res/ui/9.png");
	//tex->Save(L"testNum.xml");
	//tex->Load(L"testNum.xml");

	m_DebugCamera = std::make_shared<UICamera>();
	m_DebugCamera->CreateLookAt({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	m_DebugCamera->m_fCameraPitch = 0.0f;
	m_DebugCamera->CreateOrthographic((float)LGlobal::g_WindowWidth, (float)LGlobal::g_WindowHeight, 0, 1);
	LGlobal::g_pUICamera = m_DebugCamera.get();

	//m_DebugCamera = std::make_shared<LDebugCamera>();
	//m_DebugCamera->CreateLookAt({ 0.0f, 200.0f, -100.0f }, { 0.0f, 0.0f, 1.0f });
	////m_DebugCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);
	//m_DebugCamera->CreateOrthographic((float)LGlobal::g_WindowWidth, (float)LGlobal::g_WindowHeight, 0, 1);
	//LGlobal::g_pMainCamera = m_DebugCamera.get();


	animInfo info;
	info.isLoop = true;
	info.name = L"Anim";
	info.keyFrames.push_back({ L"../../res/effect/inhaleeffect12.png",1.f });
	info.keyFrames.push_back({ L"../../res/effect/star3.png",1.f });

	animInfo info2;
	info2.isLoop = true;
	info2.name = L"Anim2";
	info2.keyFrames.push_back({ L"../../res/effect/inhaleeffect12.png",0.1f });
	info2.keyFrames.push_back({ L"../../res/effect/inhaleeffect12.png",0.1f });
	info2.keyFrames.push_back({ L"../../res/effect/star3.png",0.1f });
	info2.keyFrames.push_back({ L"../../res/effect/inhaleeffect12.png",0.1f });






	////obj1
	//{
	//	sObj = make_shared<KObject>();
	////scripts
	//sObj->AddScripts(make_shared<PickingUI>());
	//sObj->AddScripts(make_shared<Resize2D>());
	//sObj->AddScripts(make_shared<DragUI>());
	////sObj->AddScripts(make_shared<ImguiDetail>());
	//sObj->AddScripts(make_shared<Animator>(L"Anim2.xml"));
	//sObj->AddScripts(make_shared<ChangeTexture>());
	//sObj->SetPos({ 80,80,0 });
	//sObj->SetScale({ 200,200,1 });
	//sObj->SetRect(sObj->m_vPosition, sObj->m_vScale);
	//sObj->Init();
	//sObj->SetName(L"Button1");
	//sObj->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/ui/Empty.png");
	////sObj->GetScript<Animator>(L"Animator")->CreateAnimation(info2);

	//UIManager::GetInstance().AddUIObject(sObj);
	//}
	////obj2
	//{
	//	sObj2 = make_shared<KObject>();
	//	//scripts
	//	sObj2->AddScripts(make_shared<PickingUI>());
	//	sObj2->AddScripts(make_shared<Resize2D>());
	//	sObj2->AddScripts(make_shared<DragUI>());
	//	//sObj2->AddScripts(make_shared<ImguiDetail>());
	//	sObj2->AddScripts(make_shared<ChangeTexture>());
	//	sObj2->AddScripts(make_shared<DigitDisplay>(3, L"testNum.xml"));
	//	sObj2->SetPos({ 200,200,0 });
	//	sObj2->SetScale({ 200,200,1 });
	//	sObj2->SetRect(sObj->m_vPosition, sObj->m_vScale);
	//	sObj2->Init();
	//	sObj2->SetName(L"Button2");
	//	sObj2->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/ui/Empty.png");

	//	UIManager::GetInstance().AddUIObject(sObj2);
	//}
	////obj3
	//{
	//	obj = make_shared< KObject>();
	//	obj->AddScripts(make_shared<PickingUI>());
	//	obj->AddScripts(make_shared<Resize2D>());
	//	obj->AddScripts(make_shared<DragUI>());
	//	//obj->AddScripts(make_shared<ImguiDetail>());
	//	obj->AddScripts(make_shared<ChangeTexture>());
	//	//obj->AddScripts(make_shared<ExitWindow>());
	//	obj->Set();
	//	obj->SetName(L"Button3");
	//	obj->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/ui/8.png");
	//
	//	obj->Init();
	//	obj->SetPos({ 100,100,1 });
	//	obj->SetScale({ 100,100,10 });
	//	UIManager::GetInstance().AddUIObject(obj);
	//}



	return true;
}

bool Sample::Frame()
{




	UIManager::GetInstance().Frame();



	//sObj2->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber();

	return true;
}

bool Sample::Render()
{

	UIManager::GetInstance().Render();

	return true;
}

bool Sample::Release()
{
	return true;
}

Sample::~Sample() {}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR IpCmdLine, int nCmdShow)
{
	Sample win;
	win.SetRegisterWindowClass(hInstance);
	win.SetCreateWindow(L"TeamBProject", (float)LGlobal::g_WindowWidth, (float)LGlobal::g_WindowHeight);
	win.Run();
}


