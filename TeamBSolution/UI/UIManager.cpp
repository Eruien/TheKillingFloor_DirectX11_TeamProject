#include "UIManager.h"
#include "tinyxml2.h"
#include "LGlobal.h"

shared_ptr<KObject> UIManager::s_selectedObject = nullptr;
bool UIManager::s_isMouseInImGuiWindow = false;
void UIManager::Init()
{
    _imGuiManager = make_shared< ImGuiManager>();
    _imGuiObjDetail = make_shared<Imgui_ObjectDetail>();
    _imgui_menuBar = make_shared<imgui_menuBar>();

    _imGuiManager->Init();
    _imGuiObjDetail->Init();
    _imgui_menuBar->Init();
}

void UIManager::Frame()
{
    _imGuiManager->Frame();
    _imGuiObjDetail->Frame();
    _imgui_menuBar->Frame();

    
	for (auto obj : _objs)
	{
        obj->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matOrthoProjection);
        obj->Frame();
	}
}

void UIManager::Render()
{

	for (auto obj : _objs)
	{
		obj->Render();
	}

    _imGuiObjDetail->Render();
    _imgui_menuBar->Render();
    _imGuiManager->Render();


}

void UIManager::Save(const wstring filePath)
{
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = doc.NewElement("UIScene");
    root->SetAttribute("SceneName", "TestScene");
    doc.LinkEndChild(root);

    for (auto obj : _objs)
    {
        // 각 객체마다 새로운 루트 엘리먼트(UIObject)를 생성
        tinyxml2::XMLElement* objRoot = doc.NewElement("UIObject");
        root->LinkEndChild(objRoot);

        // Name
        objRoot->SetAttribute("Name", wtm(obj->GetName()).c_str());

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

        for (auto script : obj->GetScripts())
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
        }
    }


    doc.SaveFile(wtm(filePath).c_str());
}

void UIManager::Load(const wstring filePath)
{
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
                    if (mtw(scriptTypeAttr) == L"Resize2D")
                    {
                        obj->AddScripts(std::make_shared<Resize2D>());
                    }
                    if (mtw(scriptTypeAttr) == L"DragUI")
                    {
                        obj->AddScripts(std::make_shared<DragUI>());
                        obj->GetScript<DragUI>(L"DragUI")->Init();
                    }

                    if (mtw(scriptTypeAttr) == L"ChangeTexture")
                    {
                        obj->AddScripts(std::make_shared<ChangeTexture>());
                    }
                    if (mtw(scriptTypeAttr) == L"ExitWindow")
                    {
                        obj->AddScripts(std::make_shared<ExitWindow>());
                    }
                    if (mtw(scriptTypeAttr) == L"BillBoard")
                    {
                        obj->AddScripts(std::make_shared<BillBoard>());
                    }
                    // ScriptType에 따라 적절한 스크립트를 추가하고 설정
                    if (mtw(scriptTypeAttr) == L"Animator")
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
                    // 다른 ScriptType에 대한 처리도 추가 가능
                }
            }
        }

        // UIObject를 UIManager에 추가
        _objs.push_back(obj);
    }
}


shared_ptr<KObject> UIManager::GetUIObject(wstring name)
{
	for (auto obj : _objs)
	{
		if (obj->GetName() == name)
			return obj;
	}
	return nullptr;
}

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}


