#include "UIManager.h"
#include "tinyxml2.h"
shared_ptr<KObject> UIManager::s_selectedObject = nullptr;
bool UIManager::s_isMouseInImGuiWindow = false;


bool CompareObjs(const shared_ptr<KObject> obj1, const shared_ptr<KObject> obj2) {
    // z 값에 따라 비교
    return obj1->m_vPosition.z < obj2->m_vPosition.z;
}

void UIManager::Init(ComPtr<ID3D11DepthStencilState> Depth, ComPtr<ID3D11DepthStencilState> Disable)
{
    _DepthStencilState = Depth;
    _DepthStencilStateDisable = Disable;


        _imGuiManager = make_shared< ImGuiManager>();
        _imGuiObjDetail = make_shared<Imgui_ObjectDetail>();
        _imgui_menuBar = make_shared<imgui_menuBar>();

   _imGuiManager->Init();
   _imGuiObjDetail->Init();
   _imgui_menuBar->Init();
    
}

void UIManager::Frame()
{
    
   // std::sort(_objs.begin(), _objs.end(), CompareObjs);


    //??
    //if(_objsTemp.empty()|| _objsTemp.size() != _objs.size())
    //    _objsTemp = _objs;

	for (auto& obj : _objs)
	{
        if (obj->GetIsRender() )
        {
            obj->SetMatrix(nullptr, &LGlobal::g_pUICamera->m_matView, &LGlobal::g_pUICamera->m_matOrthoProjection);
            obj->Frame();
        }
	}


}

void UIManager::Render()
{
	for (auto& obj : _objs)
	{
 /*       if (obj->GetScript<UIEvent>(L"UIEvent"))
        {
            obj->GetScript<UIEvent>(L"UIEvent")->Render();
            continue;
        }*/

       // LGlobal::g_pImmediateContext->OMSetDepthStencilState(_DepthStencilStateDisable.Get(), 1);
        if(obj->GetIsRender())
		    obj->Render();
      //  LGlobal::g_pImmediateContext->OMSetDepthStencilState(_DepthStencilState.Get(), 1);
	}


    if (_editMode)
    {
        _imGuiManager->Frame();
        _imGuiObjDetail->Frame();
        _imgui_menuBar->Frame();

        _imGuiObjDetail->Render();
        _imgui_menuBar->Render();
        _imGuiManager->Render();
    }



}

void UIManager::Save(const wstring filePath)
{
   
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = doc.NewElement("UIScene");
    root->SetAttribute("SceneName", wtm(filePath).c_str());
    doc.LinkEndChild(root);

    for (auto& obj : _objs)
    {
        /*if (filePath == obj->_scene)
        {*/
            // 각 객체마다 새로운 루트 엘리먼트(UIObject)를 생성
            tinyxml2::XMLElement* objRoot = doc.NewElement("UIObject");
        root->LinkEndChild(objRoot);

        // Name
        objRoot->SetAttribute("Name", wtm(obj->GetName()).c_str());
        objRoot->SetAttribute("isRender", obj->GetIsRender());
        objRoot->SetAttribute("Group", wtm(obj->_group).c_str());
        objRoot->SetAttribute("Scene", wtm(filePath).c_str());
        // 자식 엘리먼트들 추가
        // Pos
        tinyxml2::XMLElement* posNode = doc.NewElement("Position");
        objRoot->LinkEndChild(posNode);
        posNode->SetAttribute("x", obj->m_vPosition.x);
        posNode->SetAttribute("y", obj->m_vPosition.y);
        posNode->SetAttribute("z", obj->m_vPosition.z);

        // Scale
        tinyxml2::XMLElement* scaleNode = doc.NewElement("Scale");
        objRoot->LinkEndChild(scaleNode);
        scaleNode->SetAttribute("width", obj->m_vScale.x);
        scaleNode->SetAttribute("height", obj->m_vScale.y);

        // Texture
        tinyxml2::XMLElement* texNode = doc.NewElement("Texture");
        objRoot->LinkEndChild(texNode);
        texNode->SetAttribute("texPath", wtm(obj->m_Tex->m_texPath).c_str());

        // Scripts
        tinyxml2::XMLElement* scriptsNode = doc.NewElement("Scripts");
        objRoot->LinkEndChild(scriptsNode);

        for (auto& script : obj->GetScripts())
        {
            tinyxml2::XMLElement* scriptNode = doc.NewElement("Script");
            scriptsNode->LinkEndChild(scriptNode);

            scriptNode->SetAttribute("Type", wtm(script->GetName()).c_str());

            if (script->GetName() == L"Animator")
            {
                tinyxml2::XMLElement* argsNode = doc.NewElement("args");
                scriptNode->LinkEndChild(argsNode);
                argsNode->SetAttribute("animationPath", wtm(obj->GetScript<Animator>(L"Animator")->GetCurrentAnimation()->GetPath()).c_str());
            }

            if (script->GetName() == L"DigitDisplay")
            {
                tinyxml2::XMLElement* argsNode = doc.NewElement("args");
                scriptNode->LinkEndChild(argsNode);
                argsNode->SetAttribute("DigitNumber", obj->GetScript<DigitDisplay>(L"DigitDisplay")->GetDigitNum());
                argsNode->SetAttribute("TexListXml", wtm(obj->GetScript<DigitDisplay>(L"DigitDisplay")->GetXmlPath()).c_str());
            }
            if (script->GetName() == L"ButtonAction")
            {
                tinyxml2::XMLElement* argsNode = doc.NewElement("args");
                scriptNode->LinkEndChild(argsNode);
                argsNode->SetAttribute("TexListXml", wtm(obj->GetScript<ButtonAction>(L"ButtonAction")->GetXmlPath()).c_str());
                argsNode->SetAttribute("Function", wtm(obj->GetScript<ButtonAction>(L"ButtonAction")->_function).c_str());
            }
            if (script->GetName() == L"Text")
            {
                tinyxml2::XMLElement* argsNode = doc.NewElement("args");
                scriptNode->LinkEndChild(argsNode);
                argsNode->SetAttribute("Text", wtm(obj->GetScript<Text>(L"Text")->GetText()).c_str());
            }
            if (script->GetName() == L"UIEvent")
            {
                tinyxml2::XMLElement* argsNode = doc.NewElement("args");
                scriptNode->LinkEndChild(argsNode);
                argsNode->SetAttribute("Function", wtm(obj->GetScript<UIEvent>(L"UIEvent")->_function).c_str());
            }
            if (script->GetName() == L"TextToTexture")
            {
                tinyxml2::XMLElement* argsNode = doc.NewElement("args");
                scriptNode->LinkEndChild(argsNode);
                argsNode->SetAttribute("TextToTexture", wtm(obj->GetScript<TextToTexture>(L"TextToTexture")->GetText()).c_str());
                argsNode->SetAttribute("TexListXml", wtm(obj->GetScript<TextToTexture>(L"TextToTexture")->GetXmlPath()).c_str());
            }
        }
    //}
    }


    doc.SaveFile(wtm(filePath).c_str());
}

void UIManager::Load(const wstring filePath)
{
    //_objs.clear();
    s_selectedObject = nullptr;

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(wtm(filePath).c_str()) != tinyxml2::XML_SUCCESS)
    {
        // 파일을 열지 못하면 로그를 출력하고 함수 종료
        std::cerr << "Error: Unable to open XML file for loading." << std::endl;
        return;
    }

    // 루트 엘리먼트 확인
    tinyxml2::XMLElement* root = doc.FirstChildElement("UIScene");
    if (!root)
    {
        std::cerr << "Error: Missing root element 'UIScene' in XML file." << std::endl;
        return;
    }

    // 각 UIObject 엘리먼트에 대한 처리
    for (tinyxml2::XMLElement* objElement = root->FirstChildElement("UIObject"); objElement; objElement = objElement->NextSiblingElement("UIObject"))
    {
        // UIObject 생성 및 설정
        auto obj = std::make_shared<KObject>();
        obj->Init();
        // Name
        const char* nameAttr = objElement->Attribute("Name");
        if (nameAttr)
            obj->SetName(mtw(nameAttr));
        //isRender
        const char* isRender = objElement->Attribute("isRender");

        if (strcmp(isRender, "true") == 0)
        {
            obj->SetIsRender(true);
        }
        else
        {
            obj->SetIsRender(false);
        }
        //Group
        obj->_group = mtw(objElement->Attribute("Group"));
        obj->_scene = mtw(objElement->Attribute("Scene"));

        // Position
        tinyxml2::XMLElement* posElement = objElement->FirstChildElement("Position");
        if (posElement)
        {
            float x, y, z;
            posElement->QueryFloatAttribute("x", &x);
            posElement->QueryFloatAttribute("y", &y);
            posElement->QueryFloatAttribute("z", &z);
            obj->SetPos({ x, y, z });
        }

        // Scale
        tinyxml2::XMLElement* scaleElement = objElement->FirstChildElement("Scale");
        if (scaleElement)
        {
            float width, height;
            scaleElement->QueryFloatAttribute("width", &width);
            scaleElement->QueryFloatAttribute("height", &height);
            obj->SetScale({ width, height, 1.0f });
        }

        // Texture
        tinyxml2::XMLElement* texElement = objElement->FirstChildElement("Texture");
        if (texElement)
        {
            const char* texPathAttr = texElement->Attribute("texPath");
            if (texPathAttr)
                obj->Create(L"../../res/hlsl/CustomizeMap.hlsl", mtw(texPathAttr));
        }

        // Scripts
        tinyxml2::XMLElement* scriptsElement = objElement->FirstChildElement("Scripts");
        if (scriptsElement)
        {
            for (tinyxml2::XMLElement* scriptElement = scriptsElement->FirstChildElement("Script"); scriptElement; scriptElement = scriptElement->NextSiblingElement("Script"))
            {
                const char* scriptTypeAttr = scriptElement->Attribute("Type");
                if (scriptTypeAttr)
                {
                    if (mtw(scriptTypeAttr) == L"PickingUI")
                    {
                        obj->AddScripts(std::make_shared<PickingUI>());
                    }
                    else if (mtw(scriptTypeAttr) == L"Resize2D")
                    {
                        obj->AddScripts(std::make_shared<Resize2D>());
                    }
                    else  if (mtw(scriptTypeAttr) == L"DragUI")
                    {
                        obj->AddScripts(std::make_shared<DragUI>());
                        obj->GetScript<DragUI>(L"DragUI")->Init();
                    }

                    else if (mtw(scriptTypeAttr) == L"ChangeTexture")
                    {
                        obj->AddScripts(std::make_shared<ChangeTexture>());
                    }
                    else if (mtw(scriptTypeAttr) == L"ExitWindow")
                    {
                        obj->AddScripts(std::make_shared<ExitWindow>());
                    }
                    else if (mtw(scriptTypeAttr) == L"BillBoard")
                    {
                        obj->AddScripts(std::make_shared<BillBoard>());
                    }
                    // ScriptType에 따라 적절한 스크립트를 추가하고 설정
                    else if (mtw(scriptTypeAttr) == L"Animator")
                    {
                        // 여기에 적절한 Animator 생성자 인자를 추가
                        tinyxml2::XMLElement* argsElement = scriptElement->FirstChildElement("args");

                        if (argsElement)
                        {
                            const char* animationPathAttr = argsElement->Attribute("animationPath");
                            obj->AddScripts(std::make_shared<Animator>(mtw(animationPathAttr)));
                        }
                    }
                    else if (mtw(scriptTypeAttr) == L"DigitDisplay")
                    {
                        // 여기에 적절한 DigitDisplay 생성자 인자를 추가
                        tinyxml2::XMLElement* argsElement = scriptElement->FirstChildElement("args");
                        if (argsElement)
                        {
                            int digitNumber = 0;
                            const char* digitNumberAttr = argsElement->Attribute("DigitNumber");
                                digitNumber = std::atoi(digitNumberAttr);
                            const char* texListXmlAttr = argsElement->Attribute("TexListXml");
                            obj->AddScripts(std::make_shared<DigitDisplay>(digitNumber, mtw(texListXmlAttr)));
                            obj->GetScript<DigitDisplay>(L"DigitDisplay")->Init();

                        }
                    }
                    else if (mtw(scriptTypeAttr) == L"ButtonAction")
                    {
                        tinyxml2::XMLElement* argsElement = scriptElement->FirstChildElement("args");
                        if (argsElement)
                        {
                            const char* texListXmlAttr = argsElement->Attribute("TexListXml");
                            const char* bFunction = argsElement->Attribute("Function");
                            obj->AddScripts(std::make_shared<ButtonAction>(mtw(texListXmlAttr), mtw(bFunction)));
                           // obj->GetScript<ButtonAction>(L"ButtonAction")->Init();
                        }
                    }
                    else if (mtw(scriptTypeAttr) == L"Text")
                    {
                        tinyxml2::XMLElement* argsElement = scriptElement->FirstChildElement("args");
                        if (argsElement)
                        {
                            const char* textAttr = argsElement->Attribute("Text");
                            if (textAttr)
                            {
                                obj->AddScripts(std::make_shared<Text>(mtw(textAttr)));
                            }
                        }
                    }
                    else if (mtw(scriptTypeAttr) == L"HpBar")
                    {
                        obj->AddScripts(std::make_shared<HpBar>());
                        obj->GetScript<HpBar>(L"HpBar")->Init();
                    }
                    else if (mtw(scriptTypeAttr) == L"UIEvent")
                    {
                        tinyxml2::XMLElement* argsElement = scriptElement->FirstChildElement("args");
                        if (argsElement)
                        {
                            const char* bFunction = argsElement->Attribute("Function");
                            obj->AddScripts(std::make_shared<UIEvent>(mtw(bFunction)));
                             obj->GetScript<UIEvent>(L"UIEvent")->Init();
                        }
                    }
                    else if (mtw(scriptTypeAttr) == L"TextToTexture")
                    {
                        // 여기에 적절한 DigitDisplay 생성자 인자를 추가
                        tinyxml2::XMLElement* argsElement = scriptElement->FirstChildElement("args");
                        if (argsElement)
                        {
                           // wstring textTemp = L"None";
                            const char* TextAttr = argsElement->Attribute("TextToTexture");
                           // digitNumber = std::atoi(digitNumberAttr);
                            const char* texListXmlAttr = argsElement->Attribute("TexListXml");
                            obj->AddScripts(std::make_shared<TextToTexture>(mtw(TextAttr), mtw(texListXmlAttr)));
                            obj->GetScript<TextToTexture>(L"TextToTexture")->Init();

                        }
                    }
                }
            }
        }

        // UIObject를 UIManager에 추가
        _objs.push_back(obj);
      
    }
}


shared_ptr<KObject> UIManager::GetUIObject(wstring name)
{
	for (auto& obj : _objs)
	{
		if (obj->GetName() == name)
			return obj;
	}
	return nullptr;
}

vector<shared_ptr<KObject>> UIManager::GetGroup(wstring groupName)
{
    vector<shared_ptr<KObject>> group;
    for (auto& obj : _objs)
    {
        if (obj->_group == groupName)
            group.push_back(obj);
    }

     return group;

}

vector<shared_ptr<KObject>> UIManager::GetSceneObj(wstring sceneName)
{
    vector<shared_ptr<KObject>> sceneObj;
    for (auto& obj : _objs)
    {
        if (obj->_scene == sceneName)
            sceneObj.push_back(obj);
    }
    return sceneObj;
}

void UIManager::ChangeScene(Event Scene)
{
    if (Scene == Event::GOMAINSCENE)
    {
        //UIManager::GetInstance().Load(L"MainScene.xml");
        LSoundMgr::GetInstance().GetPtr(L"BackgroundSound.mp3")->Play();
        for (auto& obj : _objs)
        {
            obj->SetIsRender(false);
            if (obj->_scene == L"MainScene.xml")
            {
                obj->SetIsRender(true);
            }
        }
        auto group = UIManager::GetInstance().GetGroup(L"MainOptionMenu");
        for (auto& obj : group)
        {
            obj->SetIsRender(false);
        }
 
    
    }
    else if (Scene == Event::GOINGAMESCENE)
    {

        LInput::GetInstance().CursorChange();
       
        
        LSoundMgr::GetInstance().GetPtr(L"BackgroundSound.mp3")->Stop();
        LSoundMgr::GetInstance().GetPtr(L"InGameSound.mp3")->Play();
        LSoundMgr::GetInstance().GetPtr(L"zombieSound.mp3")->Play();



        for (auto& obj : _objs)
        {
            obj->SetIsRender(false);
            if (obj->_scene == L"IngameScene.xml")
            {
                obj->SetIsRender(true);
            }
        }
        UIManager::GetInstance().GetUIObject(L"ScreenBlood")->GetScript<UIEvent>(L"UIEvent")->_CTime = 1;
        if (LGlobal::g_PlayerModel->m_Type == PlayerType::GUN)
        {
            UIManager::GetInstance().GetUIObject(L"Selected_Sword")->SetIsRender(false);
            UIManager::GetInstance().GetUIObject(L"C_Ammo")->SetIsRender(true);
            UIManager::GetInstance().GetUIObject(L"T_Ammo")->SetIsRender(true);
            UIManager::GetInstance().GetUIObject(L"weaponUI")->SetIsRender(true);
            UIManager::GetInstance().GetUIObject(L"AmmoSlash")->SetIsRender(true);

            UIManager::GetInstance().GetUIObject(L"C_Ammo")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LWeaponMgr::GetInstance().m_map[LGlobal::g_PlayerModel->m_CurrentGun]->m_GunSpec.CurrentAmmo);
            UIManager::GetInstance().GetUIObject(L"T_Ammo")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LWeaponMgr::GetInstance().m_map[LGlobal::g_PlayerModel->m_CurrentGun]->m_GunSpec.TotalAmmo);
        }
        else
        {
            UIManager::GetInstance().GetUIObject(L"Selected_Sword")->SetIsRender(true);
            UIManager::GetInstance().GetUIObject(L"C_Ammo")->SetIsRender(false);
            UIManager::GetInstance().GetUIObject(L"T_Ammo")->SetIsRender(false);
            UIManager::GetInstance().GetUIObject(L"weaponUI")->SetIsRender(false);
            UIManager::GetInstance().GetUIObject(L"AmmoSlash")->SetIsRender(false);
            
            
        }
        UIManager::GetInstance().GetUIObject(L"RushEffect")->SetIsRender(false);


        if (LScene::GetInstance().GetState() != State::SHOPSCENE)
        {
            
            LScene::GetInstance().m_pActionList[State::INGAMESCENE]->Retry();
           /* UIManager::GetInstance().GetUIObject(L"T_Ammo")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Gun->m_GunSpec.CurrentAmmo);
            UIManager::GetInstance().GetUIObject(L"C_Ammo")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Gun->m_GunSpec.TotalAmmo);*/

        }
       
       
    }
    else if (Scene == Event::GOENDSCENE)
    {
        LScene::GetInstance().m_pActionList[State::ENDSCENE]->Init();
        LInput::GetInstance().CursorChange();
        LSoundMgr::GetInstance().GetPtr(L"InGameSound.mp3")->Stop();
        LSoundMgr::GetInstance().GetPtr(L"zombieSound.mp3")->Stop();
        LScene::GetInstance().m_pActionList[State::INGAMESCENE]->Retry();
        //UIManager::GetInstance().Load(L"EndScene.xml");
        for (auto& obj : _objs)
        {
            obj->SetIsRender(false);
            if (obj->_scene == L"EndScene.xml")
            {
                obj->SetIsRender(true);
            }
        }

       
    }
    else if (Scene == Event::GOSHOPSCENE)
    {
        LInput::GetInstance().CursorChange();
        //UIManager::GetInstance().Load(L"EndScene.xml");
        

        if (LGlobal::g_PlayerModel->m_Type == PlayerType::GUN)
        {
            for (auto& obj : _objs)
            {
                obj->SetIsRender(false);
                if (obj->_scene == L"Shop1.xml")
                {
                    obj->SetIsRender(true);
                }
            }
        }

        if (LGlobal::g_PlayerModel->m_Type == PlayerType::SWORD)
        {
            for (auto& obj : _objs)
            {
                obj->SetIsRender(false);
                if (obj->_scene == L"Shop2.xml")
                {
                    obj->SetIsRender(true);
                }
            }
        }
        LScene::GetInstance().SetTransition(Scene);
        LScene::GetInstance().m_pAction->Init();
        return;
    }
    else if (Scene == Event::GOSELECTSCENE)
    {
        
        for (auto& obj : _objs)
        {
            obj->SetIsRender(false);
            if (obj->_scene == L"SelectScene.xml")
            {
                obj->SetIsRender(true);
            }
        }
        LScene::GetInstance().m_pActionList[State::SELECTSCENE]->Init();
    }
    LScene::GetInstance().SetTransition(Scene);
}

void UIManager::RemoveObject(wstring name)
{
    _objs.erase(
        std::remove_if(_objs.begin(), _objs.end(),
            [&name](const std::shared_ptr<KObject>& obj) {
                return obj->GetName() == name;
            }
        ),
        _objs.end()
    );
}

void UIManager::UpdateResolution(int width, int height)
{
   
    for (auto& obj : _objs)
    {
        obj->m_vPosition.x *= float(width) / _beforeWidth;
        obj->m_vPosition.y *= float(height) / _beforeHeight;
        obj->m_vScale.x *= float(width) / _beforeWidth;
        obj->m_vScale.y *= float(height) / _beforeHeight;
    }   
    _beforeWidth = width;
    _beforeHeight = height;
}



UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}


