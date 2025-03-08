#pragma once
#include "LManagerAgr.h"
#include "LManager.h"
#include "ResourceBase.h"
struct Keyframe
{
	std::wstring texFilePath;
	float time = 0.f;
};
class Animation : public ResourceBase
{
public:
	Animation();
	virtual ~Animation();



	void SetLoop(bool loop) { _loop = loop; }
	bool IsLoop() { return _loop; }

	void SetTexture(shared_ptr<LTexture> texture) { _texture = texture; }
	shared_ptr<LTexture> GetTexture() { return _texture; }

	const Keyframe& GetKeyframe(INT32 index);
	INT32 GetKeyframeCount();
	void AddKeyframe(const Keyframe& keyframe);
	void Load(const wstring& path);
	void Save(const wstring& path);
	void SetKeyframes(vector<Keyframe> keyframes) { _keyframes = keyframes; };


private:
	bool _loop = false;
	shared_ptr<LTexture> _texture;
	vector<Keyframe> _keyframes;

};

