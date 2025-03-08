#include "TextureList.h"
#include "tinyxml2.h"
TextureList::TextureList() : ResourceBase(ResourceType::TextureList)
{
}

TextureList::~TextureList()
{
}

void TextureList::Save(wstring path)
{

    tinyxml2::XMLDocument doc;

    tinyxml2::XMLElement* root = doc.NewElement("TextureList");
    doc.LinkEndChild(root);

  ;
    root->SetAttribute("Name", wtm(GetName()).c_str());
  
    
    for (int i=0; i < _texList.size(); i++)
    {
        tinyxml2::XMLElement* node = doc.NewElement("TexPaths");
        root->LinkEndChild(node);
        node->SetAttribute("texFilePath", wtm(_texList[i]).c_str());
   

    }

    string pathStr(path.begin(), path.end());
    auto result = doc.SaveFile(pathStr.c_str());
}

void TextureList::Load(wstring path)
{
    tinyxml2::XMLDocument doc;

    string pathStr(path.begin(), path.end());
    auto result = doc.LoadFile(pathStr.c_str());

    if (result != tinyxml2::XML_SUCCESS) {
        // 실패한 경우 에러 처리를 수행할 수 있습니다.
        // 예: throw std::runtime_error("Failed to load XML file");
        return;
    }

    tinyxml2::XMLElement* root = doc.FirstChildElement("TextureList");
    if (!root) {
        // XML 파일의 형식이 올바르지 않음. 에러 처리를 수행할 수 있습니다.
        // 예: throw std::runtime_error("Invalid XML file format");
        return;
    }

    // TextureList의 이름을 불러오기
    const char* nameAttribute = root->Attribute("Name");
    if (nameAttribute) {
        SetName(mtw(nameAttribute));
    }

    // 텍스처 경로 노드들을 순회하며 텍스처 경로들을 불러오기
    tinyxml2::XMLElement* texPathsNode = root->FirstChildElement("TexPaths");
    while (texPathsNode) {
        const char* texFilePathAttribute = texPathsNode->Attribute("texFilePath");
        if (texFilePathAttribute) {
            AddTexture(mtw(texFilePathAttribute));
        }

        texPathsNode = texPathsNode->NextSiblingElement("TexPaths");
    }
}
