#include "Animator.h"
#include "LGlobal.h"
#include "KObject.h"
Animator::Animator(wstring path) : MonoBehaviour(L"Animator")
{
	_currentAnimation = make_shared< Animation>();
	_currentAnimation->Load(path);
}

Animator::~Animator()
{
}

void Animator::Init()
{
}

void Animator::Frame()
{
	shared_ptr<Animation> animation = GetCurrentAnimation();


	if (animation == nullptr)
		return;

	const Keyframe& keyframe = animation->GetKeyframe(_currentKeyframeIndex);

	float deltaTime = LGlobal::g_fSPF;
	_sumTime += deltaTime;

	if (_sumTime >= keyframe.time)
	{
		_currentKeyframeIndex++;
		

		
		
		INT32 totalCount = animation->GetKeyframeCount();

		if (_currentKeyframeIndex >= totalCount)
		{
			if (animation->IsLoop())
				_currentKeyframeIndex = 0;
			else
				_currentKeyframeIndex = totalCount - 1;
		}
		GetGameObject()->m_Tex = LManager<LTexture>::GetInstance().Load(GetCurrentKeyframe().texFilePath);
		GetGameObject()->m_Tex->Apply();

		_sumTime = 0.f;
	}

}

void Animator::CreateAnimation(animInfo info)
{

	shared_ptr<Animation> animation = make_shared< Animation>();
	animation->SetName(info.name);
	animation->SetLoop(info.isLoop);
	for (const auto& keyframe : info.keyFrames) {
		LTexture* tex = LManager<LTexture>::GetInstance().Load(keyframe.texFilePath);
		animation->AddKeyframe(Keyframe{ keyframe.texFilePath , keyframe.time });
	}
	LManager<Animation>::GetInstance().Add(info.name, animation.get());

	shared_ptr<Animation> temp = make_shared<Animation>(*LManager<Animation>::GetInstance().GetPtr(info.name));
	SetAnimation(temp);
	animation->Save(info.name + L".xml");
}
shared_ptr<Animation> Animator::GetCurrentAnimation()
{
	return _currentAnimation;
}

void Animator::SetAnimationFromXml(wstring path)
{
	_currentAnimation->Load(path);
}
