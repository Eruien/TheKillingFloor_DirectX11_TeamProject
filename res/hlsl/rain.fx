///////////////////////////// Stream Output SO Particle ////////////////////////////

float3 RandomTex(float offset)
{
	float u = offset;
	float v = g_RandomTex.SampleLevel(g_LinearSmp, offset, 0.f, 0).r;

	float4 vColor = g_RandomTex.SampleLevel(g_LinearSmp, u, v, 0);
	float3 vRandvec = vColor.xyz;

	return vRandvec;
}

GS_SO_INPUT_PARTICLE SOParticleVS(GS_SO_INPUT_PARTICLE input)
{
	return input;
}

// 스트림 출력 셰이더에서는 파티클 입자 생성 파괴 규칙을 작성한다.
// 방출 입자는 하나를 유지한다.
[maxvertexcount(2)]
void SOParticleGS(point GS_SO_INPUT_PARTICLE input[1],
	inout PointStream<GS_SO_INPUT_PARTICLE> stream)
{
	input[0].fAge += g_fTime;

	if (input[0].fType == 0.0f)
	{
		// 0.4초마다 한번씩 입자를 생성한다.
		if (input[0].fAge >= 0.4f)
		{
			GS_SO_INPUT_PARTICLE particle;

			float t = RandomTex(g_fPassTime).x;
			t -= 0.5f;
			float size = RandomTex(t).x;

			particle.fAge = 0.0f;
			particle.vSize = float2(0.3f, 0.3f) + 0.3 * size;

			particle.vPos = g_vParticleCenter + float3(5 * t, size, 0.f);
			particle.fType = 1.0f + t;

			stream.Append(particle);
			input[0].fAge = 0.0f;
		}
		stream.Append(input[0]);
	}
	else
	{
		if (input[0].fAge <= g_fParticleTime)
		{
			float pos = input[0].fType - 1;
			input[0].vPos += float3(pos, pos + 1.f, 0.f) * g_fTime * 0.5f;
			stream.Append(input[0]);
		}
	}
}

PS_OUTPUT_SINGLE SOParticlePS(GS_SO_INPUT_PARTICLE input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;
	return output;
}

////////////////////////////// Rendering SO Particle /////////////////////////////////////

GS_SO_INPUT_PARTICLE SODrawParticleVS(GS_SO_INPUT_PARTICLE input)
{
	GS_SO_INPUT_PARTICLE output = input;
	return input;
}

[maxvertexcount(4)]
void SODrawParticleGS(point GS_SO_INPUT_PARTICLE input[1],
	inout TriangleStream<GS_SO_OUTPUT_PARTICLE> stream)
{
	float	fHalfX = 0.5f * input[0].vSize.x;
	float	fHalfY = 0.5f * input[0].vSize.y;

	float3	vPos[4];
	vPos[0] = input[0].vPos - g_vCameraAxisX * fHalfX - g_vCameraAxisY * fHalfY;
	vPos[1] = input[0].vPos - g_vCameraAxisX * fHalfX + g_vCameraAxisY * fHalfY;
	vPos[2] = input[0].vPos + g_vCameraAxisX * fHalfX - g_vCameraAxisY * fHalfY;
	vPos[3] = input[0].vPos + g_vCameraAxisX * fHalfX + g_vCameraAxisY * fHalfY;

	float2	vUV[4] =
	{
		float2(0.f, 1.f),
		float2(0.f, 0.f),
		float2(1.f, 1.f),
		float2(1.f, 0.f)
	};

	GS_SO_OUTPUT_PARTICLE	output;

	for (int i = 0; i < 4; ++i)
	{
		output.vProjPos = mul(float4(vPos[i], 1.f), g_matVP);
		output.vPos = output.vProjPos;
		output.vUV = ComputeAnimation2D(vUV[i]);
		output.fAlpha = input[0].fAge;
		stream.Append(output);
	}
}

PS_OUTPUT_SINGLE SODrawParticlePS(GS_SO_OUTPUT_PARTICLE input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	float4 vColor = (float4)0;
	float2 vUV = input.vUV;

	// 애니메이션이 없을때
	if (g_iAnimation2DEnable == 0)
		vColor = g_DiffuseTex.Sample(g_LinearSmp, vUV);

	else
	{
		if (g_iAnim2DType == AT_ATLAS)
			vColor = g_DiffuseTex.Sample(g_LinearSmp, vUV);

		else
			vColor = g_ArrayTex.Sample(g_LinearSmp, float3(vUV, g_iAnim2DFrame));
	}

	if (vColor.a == 0.f)
		clip(-1);

	// 이 사각형이 출력될 좌표를 이용하여 화면에서의 UV를 만들어낸다.
	float2	vScreenUV;

	vScreenUV.x = (input.vProjPos.x / input.vProjPos.w) * 0.5f + 0.5f;
	vScreenUV.y = (input.vProjPos.y / input.vProjPos.w) * -0.5f + 0.5f;

	// 여기서 소프트파티클 처리를 한다.
	float4	vDepth = g_GBufferDepth.Sample(g_PointSmp, vScreenUV);

	float	fAlpha = vDepth.w - input.vProjPos.w;

	if (fAlpha > 0.f)
		fAlpha = fAlpha / 0.5f;

	else
		fAlpha = 1.f;

	fAlpha = min(fAlpha, 1.f);

	vColor *= g_vMtrlDif;
	vColor.a *= fAlpha;
	vColor.a *= 1.5f - input.fAlpha / 2.f;

	output.vColor = vColor;

	return output;
}