#pragma once
#include "LStd.h"
#include "Utils.h"

struct CameraData
{
	TMatrix viewTMatrix = TMatrix::Identity;
	TMatrix projectionTMatrix = TMatrix::Identity;
};

struct TransformData
{
	TMatrix worldTMatrix = TMatrix::Identity;
};

struct AnimationData
{
	TVector2 spriteOffset;
	TVector2 spriteSize;
	TVector2 textureSize;
	float useAnimation;
	float padding;
};

struct KeyFrame
{
	TVector2 offset = TVector2::Zero;
	TVector2 size = TVector2::Zero;
	float time = 0.0f;
};

struct ShaderDesc
{
	ComPtr<ID3DBlob> blob;
	ComPtr<ID3DX11Effect> effect; 
};