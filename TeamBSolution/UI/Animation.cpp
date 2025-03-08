#include "Animation.h"
#include "tinyxml2.h"

Animation::Animation() : ResourceBase(ResourceType::Animation)
{

}

Animation::~Animation()
{

}




const Keyframe& Animation::GetKeyframe(INT32 index)
{
	return _keyframes[index];
}

INT32 Animation::GetKeyframeCount()
{
	return static_cast<INT32>(_keyframes.size());
}

void Animation::AddKeyframe(const Keyframe& keyframe)
{
	_keyframes.push_back(keyframe);
}
void Animation::Load(const wstring& path)
{
	tinyxml2::XMLDocument doc;

	string pathStr(path.begin(), path.end());
	tinyxml2::XMLError error = doc.LoadFile(pathStr.c_str());
	//assert(error == XMLError::XML_SUCCESS);

	tinyxml2::XMLElement* root = doc.FirstChildElement();
	string nameStr = root->Attribute("Name");
	_name = wstring(nameStr.begin(), nameStr.end());

	_loop = root->BoolAttribute("Loop");
	_path = path;

	// Load Texture

	tinyxml2::XMLElement* node = root->FirstChildElement();
	for (; node != nullptr; node = node->NextSiblingElement())
	{

		Keyframe keyframe;
		keyframe.texFilePath = mtw(node->Attribute("texFilePath"));
		keyframe.time = node->FloatAttribute("time");

 		AddKeyframe(keyframe);
	}
}

void Animation::Save(const wstring& path)
{
	tinyxml2::XMLDocument doc;

	tinyxml2::XMLElement* root = doc.NewElement("Animation");
	doc.LinkEndChild(root);


	root->SetAttribute("Name", wtm(GetName()).c_str());
	root->SetAttribute("Loop", _loop);

	for (const auto& keyframe : _keyframes)
	{
		tinyxml2::XMLElement* node = doc.NewElement("Keyframe");
		root->LinkEndChild(node);



		node->SetAttribute("texFilePath", wtm(keyframe.texFilePath).c_str());
		node->SetAttribute("time", keyframe.time);
	
	}

	string pathStr(path.begin(), path.end());
	auto result = doc.SaveFile(pathStr.c_str());
	//assert(result == XMLError::XML_SUCCESS);
}