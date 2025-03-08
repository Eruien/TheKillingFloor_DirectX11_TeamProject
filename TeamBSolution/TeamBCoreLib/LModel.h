#pragma once
#include "LFbxObj.h"
#include "LBox.h"

class LModel
{
public:
	static std::vector<LBoneWorld> m_texBoneArray;
	int m_InstanceSize = 0;
	int m_matBoneArraySize = 200;
	ComPtr<ID3D11Texture2D> pAnimationArrayTex = nullptr;
	ComPtr<ID3D11ShaderResourceView> pAnimationArraySRV = nullptr;
	std::vector<int> m_texAnimationOffset;
	std::vector<int> m_AnimationFrameList;
	int m_offsetCount = 0;
	ComPtr<ID3D11Buffer> m_pCurrentFrameCB;
	int m_CurrentFrame[4] = { 0, };
	static int m_AllAnimationFrame;
	T_BOX m_SettingBox;
	LBox m_OBBBox;
	T_BOX m_SettingBoxRightHand;
	T_BOX m_SettingBoxLeftHand;
	LBox m_OBBBoxRightHand;
	LBox m_OBBBoxLeftHand;
	float m_fRadius = 0.0f;
public:
	LFbxObj* m_pModel = nullptr;
	LFbxObj* m_pActionModel = nullptr;
	LFbxObj* m_pComparePtr = nullptr;
public:
	float m_fCurrentAnimTime = 0.0f;
	LBoneWorld m_matBoneArray;
	LBoneWorld m_matMeshBoneArray;
	ComPtr<ID3D11Buffer> m_pBoneArrayCB = nullptr;
	TMatrix m_matControl;
	std::wstring m_ParentBoneName;
	PlayerType m_Type = PlayerType::GUN;
public:
	bool m_TimerStart = false;
	bool m_TimerEnd = false;
	bool bVisible = false;
	float m_HP = 100.0f;
	float m_AnimationRate = 1.0f;

public:
	virtual void SetAnimationArrayTexture();
	virtual void SetAnimationArraySRV();
	virtual void SetInstancing(bool IsIntancing, int instancingCount);
	virtual void SetLFbxObj(LFbxObj* fbxObj);
	virtual void SetOBBBox(TVector3 min, TVector3 max, float boxScale);
	virtual void SetOBBBoxRightHand(TVector3 min, TVector3 max, float boxScale);
	virtual void SetOBBBoxLeftHand(TVector3 min, TVector3 max, float boxScale);
	virtual LFbxObj* GetLFbxObj();
public:
	TVector3 GetPosition() { return { m_matControl._41, m_matControl._42, m_matControl._43 }; };
	void SetPosition(TVector3 pos);
	void SetPosition(float x, float y, float z);


	virtual bool Init(int animationIndex);
	virtual bool ComputeOffset();
	virtual bool SetTexture();
	virtual bool CreateBoneBuffer();
	virtual bool Frame();
	virtual bool FrameInstancing();
	virtual bool Render();
	bool ShadowRender(TMatrix* matShadowWorld, TMatrix* matShadowView, TMatrix* matShadowProj);
	virtual bool Release();
};

class LSkinningModel : public LModel
{
public:
	virtual bool AllNodeRender();
	bool CreateBoneBuffer() override;
	bool CreateCurrentFrameBuffer();
	virtual bool Frame() override;
	virtual bool AniFrame();
	virtual bool Render() override;
	virtual bool AniRender();
	bool Release() override;
};