#define MAX_INSTANCING_MATRICES 255 

struct VS_INPUT
{
    float3 p : POSITION;
    float3 n : NORMAL;
    float4 c : COLOR;
    float2 t : TEXCOORD;
    uint InstanceId : SV_InstanceID;
};

struct VS_OUTPUT
{
    float4 p : SV_POSITION;
    float3 n : NORMAL;
    float4 c : COLOR0;
    float2 t : TEXCOORD0;
};
// �������(�������� ������ ����Ǿ�� �Ѵ�.)
// 0�� world, view, proj
// 1�� �ִϸ��̼� ���
// 2�� �׸���
// 3�� �ν��Ͻ�
cbuffer cb0 
{
    matrix g_matWorld  : packoffset(c0);// float4x4 // 4��    
    matrix g_matView  : packoffset(c4);
    matrix g_matProj  : packoffset(c8);
};

cbuffer cb3 : register(b3)
{
    matrix g_matInstancing[MAX_INSTANCING_MATRICES];
};

VS_OUTPUT VS(VS_INPUT vIn)
{
    VS_OUTPUT vOut = (VS_OUTPUT)0;
    // v * m ->(��->c0,c1,c2,c3)
    // v dot c0 = v.x
    // v dot c1 = v.y
    // v dot c2 = v.z
    // v dot c3 = v.w     
    float4 vWorld = mul(float4(vIn.p,1.0f), g_matInstancing[vIn.InstanceId]);
    float4 vView = mul(vWorld, g_matView);
    float4 vProj = mul(vView, g_matProj);
    vOut.p = vProj;
    vOut.n = vIn.n;
    vOut.t = vIn.t;
    vOut.c = vIn.c;
    return vOut;
}

Texture2D g_txDiffuse1 : register(t0);
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
