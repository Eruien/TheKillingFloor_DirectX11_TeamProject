#pragma once
#include "LObject.h"
#include "MonoBehaviour.h"


class KObject : public LObject , public enable_shared_from_this<KObject>
{
public:
	virtual ~KObject() {};
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;

	void SetName(wstring name) { _name = name; };
	wstring GetName() {	return _name;};
	
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


protected:
	wstring _name;
	vector<shared_ptr<MonoBehaviour>> _scripts;
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
