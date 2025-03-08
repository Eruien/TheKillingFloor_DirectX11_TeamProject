//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
// �ؽ�ó�� ���÷� ���¸� ���̴����� ����� �� �ֵ��� �������Ϳ� ���
Texture2D g_txDiffuse : register (t0); // �⺻ �ؽ�ó
Texture2D g_txShadow1 : register (t1); // ù ��° �׸��� ��
Texture2D g_txShadow2 : register (t2); // �� ��° �׸��� ��
SamplerState g_SamLinear : register (s0); // �⺻ ���÷�
SamplerState g_samShadowMap : register (s1); // �׸��� �ʿ� ���÷�

// ��� ���۸� �����ϰ� ��ŷ �������� ����Ͽ� ������ cbuffer�� Ư�� ��ġ�� �Ҵ�
cbuffer cb0 : register (b0)
{
    matrix g_matWorld  : packoffset(c0); // ���� ���
    matrix g_matView  : packoffset(c4); // �� ���
    matrix g_matProj  : packoffset(c8); // �������� ���
};

cbuffer cb1 : register (b1)
{
    matrix g_matShadow1; // ù ��° �׸��� ���� ��ȯ ���
    matrix g_matShadow2; // �� ��° �׸��� ���� ��ȯ ���
};

// ���� ���̴� �Է� �� ��� ����ü ����
// �� ����ü���� ���� ���̴��� �Է� �� ��� �����͸� ����
struct VS_INPUT
{
    float4 p : POSITION; // ���� ��ġ
    float3 n : NORMAL; // ������ ��� ����
    float4 c : COLOR0; // ���� ����
    float2 t : TEXCOORD0; // �ؽ�ó ��ǥ
};

struct VS_OUTPUT
{
    float4 p : SV_POSITION; // ȭ����� ���� ��ġ
    float4 c : COLOR0; // ���� ����
    float2 t : TEXCOORD0; // �ؽ�ó ��ǥ
    float4 TexShadow1 : TEXCOORD1; // ù ��° �׸��� ���� �ؽ�ó ��ǥ
    float4 TexShadow2 : TEXCOORD2; // �� ��° �׸��� ���� �ؽ�ó ��ǥ
};

// ���� ���̴� �Լ�
VS_OUTPUT VS(VS_INPUT vIn)
{
    VS_OUTPUT vOut = (VS_OUTPUT)0;
    // ���� ����� ���� ���� ��ġ�� ��ȯ
    vOut.p = mul(vIn.p, g_matWorld);
    // �� ��İ� �������� ����� ���� ȭ����� ��ġ�� ��ȯ
    vOut.p = mul(vOut.p, g_matView);
    vOut.p = mul(vOut.p, g_matProj);
    // �ؽ�ó ��ǥ�� ������ �״�� ����
    vOut.t = vIn.t;
    vOut.c = vIn.c;
    // �� �׸��� ���� �ؽ�ó ��ǥ�� ���
    vOut.TexShadow1 = mul(vIn.p, g_matShadow1);
    vOut.TexShadow2 = mul(vIn.p, g_matShadow2);
    return vOut;
}


// �ȼ� ���̴� �Լ�
float4 PS(VS_OUTPUT vIn) : SV_Target
{
    // �׸��� ���� ���ø��Ͽ� �׸��� ���� ���θ� �Ǻ�
    float4 shadow1 = g_txShadow1.Sample(g_samShadowMap, vIn.TexShadow1.xy / vIn.TexShadow1.w);
    float4 shadow2 = g_txShadow2.Sample(g_samShadowMap, vIn.TexShadow2.xy / vIn.TexShadow2.w);
    // �⺻ �ؽ�ó�� ������ ���ø�
    float4 FinalColor = g_txDiffuse.Sample(g_SamLinear, vIn.t);

    // �׸��ڰ� �����ϴ� ��� ������ ��Ӱ� ó��
    if (shadow1.r > 0.01f || shadow2.r > 0.01f)
    {
        FinalColor = FinalColor * float4(0.5f, 0.5f, 0.5f, 1.0f);
        FinalColor.w = 1.0f;
    }

    // ���������� ������ 0.3��� ���� (��ο� ������ �ֱ� ����)
    FinalColor *= 0.3f;
    FinalColor.w = 1.0f;
    return FinalColor;
}
