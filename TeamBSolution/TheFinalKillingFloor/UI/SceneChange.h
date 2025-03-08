#pragma once
#include "MonoBehaviour.h"
#include "../LScene.h"
#include "LInput.h"
class SceneChange : public MonoBehaviour
{
public:
	SceneChange(Event event);
	SceneChange();
	~SceneChange();

	void Init() override;
	void Frame() override;
	void SetEvent(Event event) { _sceneEvent = event; };
	Event GetEvent() { return _sceneEvent; };
private:
	Event _sceneEvent;
};
