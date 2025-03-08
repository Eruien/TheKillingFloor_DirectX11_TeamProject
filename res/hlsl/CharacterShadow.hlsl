#define MAX_BONE_MATRICES 255
Texture2D g_txDiffuse : register(t0);
Texture2D g_txShadow1 : register(t1);
Texture2D g_txShadow2 : register(t2);
SamplerState g_SamLinear : register(s0);
SamplerState g_SamClamp : register(s1);

cbuffer cb0 : register (b0)
{
    matrix g_matWorld  : packoffset(c0);// float4x4 // 4°³    
    matrix g_matView  : packoffset(c4);
    matrix g_matProj  : packoffset(c8);
};

cbuffer cbAnimMatrices : register(b1)
{
    float4x4 g_matBoneWorld[MAX_BONE_MATRICES];
};

cbuffer cb2 : register(b2)
{
    matrix g_matShadow1;
    matrix g_matShadow2;
};

struct VS_INPUT
{
    float3 p : POSITION;
    float3 n : NORMAL;
    float4 c : COLOR;    // COLOR0 ~ COLOR1
    float2 t : TEXCOORD0; // TEXCOORD0 ~ TEXCOORD15

    float4 i : INDEX;
    float4 w : WEIGHT;
    float3 tan : TANGENT;
};

struct VS_OUTPUT
{
    float4 p : SV_POSITION;
    float3 n : NORMAL;
    float4 c : COLOR0;
    float2 t : TEXCOORD0;
    float4 TexShadow1 : TEXCOORD1;
    float4 TexShadow2 : TEXCOORD2;
};

VS_OUTPUT VS(VS_INPUT vIn)
{
    VS_OUTPUT vOut = (VS_OUTPUT)0;

    float4 vLocal = float4(vIn.p.xyz, 1.0f);
    float4 vWorld = 0;
    float3 vNormal = 0;

    for (int iBone = 0; iBone < 4; iBone++)
    {
        uint iBoneIndex = vIn.i[iBone];
        vWorld += mul(vLocal, g_matBoneWorld[iBoneIndex]) * vIn.w[iBone];
        vNormal += mul(vIn.n, (float3x3)g_matBoneWorld[iBoneIndex]) * vIn.w[iBone];
    }

    vWorld = mul(vWorld,g_matWorld);
    float4 vView = mul(vWorld, g_matView);
    float4 vProj = mul(vView, g_matProj);
    vOut.p = vProj;
    vOut.n = vNormal;
    vOut.t = vIn.t;
    vOut.c = vIn.c;

    vOut.TexShadow1 = mul(vIn.p, g_matShadow1);
    vOut.TexShadow2 = mul(vIn.p, g_matShadow2);
    return vOut;
}

struct PS_IN
{
    float4 p : SV_POSITION;
    float2 t : TEXTURE;
};

float4 PS(VS_OUTPUT vIn) : SV_Target
{
    float4 shadow1 = g_txShadow1.Sample(g_SamClamp, vIn.TexShadow1.xy / vIn.TexShadow1.w);
    float4 shadow2 = g_txShadow2.Sample(g_SamClamp, vIn.TexShadow2.xy / vIn.TexShadow2.w);
    float4 finalColor = g_txDiffuse.Sample(g_SamLinear, vIn.t) * vIn.c;

    if (shadow1.r > 0.01f || shadow2.r > 0.01f)
    {
        finalColor = finalColor * float4(0.5f, 0.5f, 0.5f, 1.0f);
        finalColor.w = 1.0f;
    }

    return finalColor;
}