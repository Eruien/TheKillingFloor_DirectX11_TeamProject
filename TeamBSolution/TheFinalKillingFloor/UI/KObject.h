#pragma once
#include "LObject.h"
#include "MonoBehaviour.h"

class KObject : public LObject , public enable_shared_from_this<KObject>
{
public:
	KObject() = default;
	KObject(const KObject& other) 
	{


	
		Init();
		Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/ui/Hud_Bio_Clock_Circle.png");
		m_Tex = other.m_Tex;
		m_Shader = other.m_Shader;
		m_Tex->Apply();
		SetPos({ 0,0,0 });
		SetScale(other.m_vScale);
		_name = other._name;
		_group = other._group;
		_scene = other._scene;
		_isRender = other._isRender;
		//_rect = other._rect;


	};
	virtual ~KObject() {};
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;

	void SetName(wstring name) { _name = name; };
	wstring GetName() {	return _name;};
	
	void SetIsRender(bool isRender) { _isRender = isRender; };
	bool GetIsRender() { return _isRender; };


	// Rect
	void SetRect(TVector3 pos, TVector3 scale);
	TRectangle GetRect() { return _rect; };
	bool CreateVertexBuffer() override;
	//스크립트
	void AddScripts(shared_ptr<MonoBehaviour> script);
	void RemoveScript(wstring name);

	template<class T>
	shared_ptr<T> GetScript(wstring name);
	vector<shared_ptr<MonoBehaviour>> GetScripts() { return _scripts;};

//Rect
public:
	TRectangle _rect;
	wstring _group = L"None";
	wstring _scene = L"None";

protected:
	wstring _name;
	vector<shared_ptr<MonoBehaviour>> _scripts;
	bool _isRender = true;
	
};

template<class T>
inline shared_ptr<T> KObject::GetScript(wstring name)
{
	for (int i = 0; i < _scripts.size(); i++)
	{
		if (name == _scripts[i]->_name)
		{
			auto temp = dynamic_pointer_cast<T>(_scripts[i]);
			
			if (temp != nullptr)
			{
				return temp;
			}
		}
	}
	return nullptr;
}
