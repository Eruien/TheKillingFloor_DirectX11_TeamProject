// VertexShader
struct VS_IN
{
    float3 p : POS;
    float2 t : TEXTURE;
};

struct VS_OUT
{
    float4 p : SV_POSITION;
    float2 t : TEXTURE;
};

cbuffer cb0
{
    matrix g_matWorld  : packoffset(c0);
    matrix g_matView  : packoffset(c4);
    matrix g_matProj  : packoffset(c8);
};

VS_OUT VS(VS_IN vertexIn)
{
    VS_OUT vertexOut = (VS_OUT)0;
    vertexOut.p = mul(float4(vertexIn.p, 1), g_matWorld);
    vertexOut.p = mul(vertexOut.p, g_matView);
    vertexOut.p = mul(vertexOut.p, g_matProj);
    vertexOut.t = vertexIn.t;
    return vertexOut;
}

//PixelShader
Texture2D g_txDiffuse1 : register(t0);
SamplerState sample0 : register(s0);

struct PS_IN
{
    float4 p : SV_POSITION;
    float2 t : TEXTURE;
};

float4 PS(PS_IN vertexIn) : SV_Target
{
    return g_txDiffuse1.Sample(sample0, vertexIn.t);
}

