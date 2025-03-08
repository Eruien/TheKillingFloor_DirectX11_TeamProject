#pragma once
#include "LStd.h"
#include "KObject.h"
#include "LObject.h"

struct RainVertex
{
	TVector3 pos;
	TVector3 seed;
	TVector3 speed;
	float random;
	unsigned char  Type;
};
struct WindValue
{
	TVector3 windAmount;
	int time;
	WindValue(TVector3 wA, int t) { windAmount = wA; time = t; };
};
vector<WindValue> WindAnimation;
int totalAnimationValues = 0;

class ParticleClass
{
	
	void InitializeRainEffect(ID3D11Device* pd3dDevice);
	void RenderRain(ID3D11DeviceContext* pd3dImmediateContext);
};

