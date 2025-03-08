struct VS_INPUT
{
    float3 p : POSITION;
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
    float4 TexShadow1 : TEXCOORD1;
    float4 TexShadow2 : TEXCOORD2;
};
// �������(�������� ������ ����Ǿ�� �Ѵ�.)
// �������� ������, float4(x,y,z,w) 
cbuffer cb0 
{
    matrix g_matWorld  : packoffset(c0);// float4x4 // 4��    
    matrix g_matView  : packoffset(c4);
    matrix g_matProj  : packoffset(c8);
};
cbuffer cb1: register (b1)
{
    float4x4 g_matShadow1;
    float4x4 g_matShadow2;
}
VS_OUTPUT VS(VS_INPUT vIn)
{
    VS_OUTPUT vOut = (VS_OUTPUT)0;
    // v * m ->(��->c0,c1,c2,c3)
    // v dot c0 = v.x
    // v dot c1 = v.y
    // v dot c2 = v.z
    // v dot c3 = v.w     
    float4 vWorld = mul(float4(vIn.p,1.0f),g_matWorld);
    float4 vView = mul(vWorld, g_matView);
    float4 vProj = mul(vView, g_matProj);
    vOut.p = vProj;
    vOut.n = vIn.n;
    vOut.t = vIn.t;
    vOut.c = vIn.c;
    //�ؽ�ó��ǥ
    vOut.TexShadow1 = mul(vIn.p, g_matShadow1);
    vOut.TexShadow2 = mul(vIn.p, g_matShadow2);
    return vOut;
}

Texture2D g_txDiffuse1 : register(t0);
Texture2D g_txShadow1: register (t1);
Texture2D g_txShadow2: register (t2);
SamplerState sample0 : register(s0);
struct PS_IN
{
    float4 p : SV_POSITION;
    float2 t : TEXTURE;
};
float4 PS(VS_OUTPUT vIn) : SV_Target
{
    //            r,g,b,a(1)=������, a(0)=��������, a(0.0< 1.0f)= ������
    return g_txDiffuse1.Sample(sample0, vIn.t);// *vIn.c;
    //return vIn.c;
}
float4 PS_Color(VS_OUTPUT vIn) : SV_Target
{
    return vIn.c;
}
