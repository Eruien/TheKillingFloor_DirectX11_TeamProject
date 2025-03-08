#pragma once
#include "MonoBehaviour.h"
#include "Animation.h"

struct animInfo
{
	wstring name;
	//wstring textureKey;
	bool isLoop;
	vector<Keyframe> keyFrames;
};

class Animator : public MonoBehaviour
{
public:
	Animator(wstring path);
	virtual ~Animator();

	void Init() override;
	void Frame() override;

	//애니메이션
	void CreateAnimation(animInfo info);
	shared_ptr<Animation> GetCurrentAnimation();
	const Keyframe& GetCurrentKeyframe() { return _currentAnimation->GetKeyframe(_currentKeyframeIndex); }
	void SetAnimation(shared_ptr<Animation> animation) { _currentAnimation = animation; }
	void SetAnimationFromXml(wstring path);
	INT32 _currentKeyframeIndex = 0;
	//애니메이션
protected:
	float _sumTime = 0.f;

	shared_ptr<Animation> _currentAnimation;
};

