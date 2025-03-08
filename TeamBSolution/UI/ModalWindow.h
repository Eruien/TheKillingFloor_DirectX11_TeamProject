#pragma once
#include "MonoBehaviour.h"
class ModalWindow : public MonoBehaviour
{
public:
	ModalWindow();
	~ModalWindow();

	void AddChild(shared_ptr<KObject> child) { _child.push_back(child);};
	void Init() override;
	void Frame() override;

private :
	vector<shared_ptr<KObject>> _child;
};

