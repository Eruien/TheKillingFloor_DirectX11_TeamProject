#pragma once
#include "LStd.h"

enum class ResourceType :  __int8
{
	None = -1,
	Animation,
	TextureList,

	End
};

enum
{
	RESOURCE_TYPE_COUNT = static_cast<__int8>(ResourceType::End)
};

class ResourceBase : public enable_shared_from_this<ResourceBase>
{
public:
	ResourceBase(ResourceType type);
	virtual ~ResourceBase();

	ResourceType GetType() { return _type; }
	wstring GetPath() { return _path; };
	void SetName(const wstring& name) { _name = name; }
	const wstring& GetName() { return _name; }
	UINT32 GetID() { return _id; }

protected:
	virtual void Load(const wstring& path) { }
	virtual void Save(const wstring& path) { }

protected:
	ResourceType _type = ResourceType::None;
	wstring _name;
	wstring _path;
	UINT32 _id = 0;
};

