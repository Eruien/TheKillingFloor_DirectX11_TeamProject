struct VS_INPUT
{
    float3 p : POSITION;
    float4 c : COLOR;
};

struct VS_OUTPUT
{
    float4 p : SV_POSITION;
    float4 c : COLOR0;
};

cbuffer cb0
{
    matrix g_matWorld : packoffset(c0);
    matrix g_matView : packoffset(c4);
    matrix g_matProj : packoffset(c8);
};

VS_OUTPUT VS(VS_INPUT vIn)
{
    VS_OUTPUT vOut = (VS_OUTPUT) 0;

    float4 vWorld = mul(float4(vIn.p, 1.0f), g_matWorld);
    float4 vView = mul(vWorld, g_matView);
    float4 vProj = mul(vView, g_matProj);
    vOut.p = vProj;
    vOut.c = vIn.c;

    return vOut;
}

float4 PS(VS_OUTPUT vIn) : SV_Target
{
    return vIn.c;
}
