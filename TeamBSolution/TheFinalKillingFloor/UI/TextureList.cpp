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
        // ������ ��� ���� ó���� ������ �� �ֽ��ϴ�.
        // ��: throw std::runtime_error("Failed to load XML file");
        return;
    }

    tinyxml2::XMLElement* root = doc.FirstChildElement("TextureList");
    if (!root) {
        // XML ������ ������ �ùٸ��� ����. ���� ó���� ������ �� �ֽ��ϴ�.
        // ��: throw std::runtime_error("Invalid XML file format");
        return;
    }

    // TextureList�� �̸��� �ҷ�����
    const char* nameAttribute = root->Attribute("Name");
    if (nameAttribute) {
        SetName(mtw(nameAttribute));
    }

    // �ؽ�ó ��� ������ ��ȸ�ϸ� �ؽ�ó ��ε��� �ҷ�����
    tinyxml2::XMLElement* texPathsNode = root->FirstChildElement("TexPaths");
    while (texPathsNode) {
        const char* texFilePathAttribute = texPathsNode->Attribute("texFilePath");
        if (texFilePathAttribute) {
            AddTexture(mtw(texFilePathAttribute));
        }

        texPathsNode = texPathsNode->NextSiblingElement("TexPaths");
    }
}
