static const int g_iNumTexture = 6;
//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
//Texture2D g_txDiffuse1 : register(t0);
TextureCube	g_txCubeMap	: register(t0);
//SamplerState        g_samPointClamp : register(s0);
//SamplerState        g_samLinearWrap : register(s1);
SamplerState sample0 : register(s0);
SamplerState g_samPointClamp : register(s1);

cbuffer cb0
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
	float4  g_MeshColor     : packoffset(c12);
};
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;

};
struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
	float3 r : TEXCOORD1;
};
struct PS_INPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
	float3 r : TEXCOORD1;
	uint PrimitiveID : SV_PrimitiveID;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT vIn)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.p = mul(vIn.p, g_matWorld);
	output.p = mul(output.p, g_matView);
	output.p = mul(output.p, g_matProj);
	output.n = vIn.n;
	output.t = vIn.t;
	output.r = normalize(vIn.p.xyz);
	output.c = vIn.c * g_MeshColor;
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT vIn) : SV_Target
{
	float depthValue = vIn.p.z / vIn.p.w;
	return vIn.c;
}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
//float4 GetTextureColor(uint index, float2 uv)
//{
//	float4 fColor = float4(1, 1, 1, 1);
//	switch (index)
//	{
//		case 0: fColor = g_txDiffuseArray[0].Sample(g_samPointClamp, uv); break;
//		case 1: fColor = g_txDiffuseArray[1].Sample(g_samPointClamp, uv); break;
//		case 2: fColor = g_txDiffuseArray[2].Sample(g_samPointClamp, uv); break;
//		case 3: fColor = g_txDiffuseArray[3].Sample(g_samPointClamp, uv); break;
//		case 4: fColor = g_txDiffuseArray[4].Sample(g_samPointClamp, uv); break;
//		case 5: fColor = g_txDiffuseArray[5].Sample(g_samPointClamp, uv); break;
//	}
//	return fColor;
//}
//
//float4 PS_TextureIndex(PS_INPUT vIn) : SV_Target
//{
//	uint index = vIn.PrimitiveID / 2;
//	float4 fColor = float4(1, 1, 1, 1);
//	fColor = GetTextureColor(index, vIn.t);
//	return fColor;
//}
//
//float4 PS_TextureArray(PS_INPUT vIn) : SV_Target
//{
//	uint index = vIn.PrimitiveID / 2;
//	float4 fColor = float4(1, 1, 1, 1);
//	fColor = g_tx2dArray.Sample(g_samPointClamp, float3(vIn.t, index));
//	return fColor;
//}

float4 PS_CubeTexture(PS_INPUT vIn) : SV_Target
{
	//float depthValue = vIn.p.z / vIn.p.w;
	float4 fColor = float4(1, 1, 1, 1);
	fColor = g_txCubeMap.Sample(g_samPointClamp, vIn.r) * 0.1;
	fColor.w = 1.0f;
	return fColor;
}