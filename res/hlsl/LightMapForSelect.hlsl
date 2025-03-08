#define WIDEN(x) g_##x
#define g_iNumLight  1
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
    float4 g_MeshColor : packoffset(c12);
};

cbuffer cb1 : register (b1)
{
    matrix g_matShadow1; // ù ��° �׸��� ���� ��ȯ ���
    matrix g_matShadow2; // �� ��° �׸��� ���� ��ȯ ���
};
cbuffer cb3: register(b3)
{
    float4				g_cAmbientMaterial[g_iNumLight];
    float4				g_cDiffuseMaterial[g_iNumLight];
    float4				g_cSpecularMaterial[g_iNumLight];
    float4				g_cEmissionMaterial[g_iNumLight];

    float4				g_cAmbientLightColor[g_iNumLight];
    float4				g_cDiffuseLightColor[g_iNumLight];
    float4				g_cSpecularLightColor[g_iNumLight];
    float                g_bIsZedTime;
    float padding[3]; // add padding
};
//--------------------------------------------------------------------------------------
//Lighting Variables
//--------------------------------------------------------------------------------------
cbuffer cb4: register(b4)
{
    matrix				g_matWorldInverse[g_iNumLight];
    float4				g_vLightDir[g_iNumLight];
    float4				g_vLightPos[g_iNumLight];
    float4				g_vEyeDir[g_iNumLight];
    float4				g_vEyePos[g_iNumLight];
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
    float3 n : NORMAL;
    float4 c : COLOR0; // ���� ����
    float2 t : TEXCOORD0; // �ؽ�ó ��ǥ
    float4 TexShadow1 : TEXCOORD1; // ù ��° �׸��� ���� �ؽ�ó ��ǥ
    float4 TexShadow2 : TEXCOORD2; // �� ��° �׸��� ���� �ؽ�ó ��ǥ
    float4 v : TEXCOORD3;
};

// ���� ���̴� �Լ�
VS_OUTPUT VS(VS_INPUT vIn)
{
    VS_OUTPUT vOut = (VS_OUTPUT)0;
    float4 vWorld = mul(vIn.p, WIDEN(matWorld));
    vOut.p = mul(vWorld, WIDEN(matView));
    vOut.p = mul(vOut.p, WIDEN(matProj));
    //vOut.n = normalize(mul(vIn.n, (float3x3)g_matWorldInverse));
    vOut.n = vIn.n;
    vOut.t = vIn.t;
    vOut.c = vIn.c; //* g_MeshColor;
    vOut.v = vWorld;
    return vOut;
}
float4 ComputePointLight(float3 vVertexPos, float3 vVertexNormal, int nNumLights)
{
    float4 vPointLightColor = float4(0, 0, 0, 1);
    for (int iLight = 0; iLight < nNumLights; iLight++)
    {
        float4 vLight;
        vLight.xyz = normalize(vVertexPos - g_vLightPos[iLight].xyz);
        vLight.w = distance(vVertexPos, g_vLightPos[iLight].xyz);

        //float fAttenuation = 1.0f / (1.0f + 0.005f * vLight.w * vLight.w); // �����Լ� �߰�
        float fLuminance = smoothstep(vLight.w - 5, vLight.w, g_vLightPos[iLight].w);
        float fIntensity = saturate(dot(vVertexNormal, -vLight.xyz));

        vPointLightColor += float4(g_cDiffuseLightColor[iLight].rgb * fLuminance, 1.0f); // �����Լ� ����
    }
    return vPointLightColor;
}
float4 Diffuse(float3 vNormal)
{
    float fIntensity = max(0, dot(vNormal, normalize(-g_vLightDir[0])));
    float4 diffuse = g_cAmbientMaterial[0] * g_cAmbientLightColor[0] +
		(g_cDiffuseMaterial[0] * g_cDiffuseLightColor[0] * fIntensity);
    return diffuse;
}


// �ȼ� ���̴� �Լ�
float4 PS(VS_OUTPUT vIn) : SV_Target
{
    float4 vTexColor = g_txDiffuse.Sample(g_SamLinear, vIn.t);
    // �׸��� ���� ���ø��Ͽ� �׸��� ���� ���θ� �Ǻ�
    float4 shadow1 = g_txShadow1.Sample(g_samShadowMap, vIn.TexShadow1.xy / vIn.TexShadow1.w);
    float4 shadow2 = g_txShadow2.Sample(g_samShadowMap, vIn.TexShadow2.xy / vIn.TexShadow2.w);
    // light
    float4 vPointLightColor = ComputePointLight(vIn.v, vIn.n, g_iNumLight);
    // �⺻ �ؽ�ó�� ������ ���ø�
    float4 vDiffuse = Diffuse(vIn.n);
    float4 FinalColor = vTexColor * (vPointLightColor + vDiffuse);

    

    // �׸��ڰ� �����ϴ� ��� ������ ��Ӱ� ó��
    if (shadow1.r > 0.01f || shadow2.r > 0.01f)
    {
        FinalColor = FinalColor * float4(0.5f, 0.5f, 0.5f, 1.0f);
        FinalColor.w = 1.0f;
    }

    float grayScale = dot(FinalColor.rgb, float3(0.299, 0.587, 0.114));
    float3 grayColor = float3(grayScale, grayScale, grayScale);
    FinalColor.rgb = lerp(FinalColor.rgb, grayColor, g_bIsZedTime); // Update this line

    return FinalColor;
}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS_Texture(VS_OUTPUT vIn) : SV_Target
{
    return g_txDiffuse.Sample(g_SamLinear, vIn.t);
}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS_Color(VS_OUTPUT vIn) : SV_Target
{
    float4 vFinalColor = ComputePointLight(vIn.v, vIn.n, g_iNumLight);
    return vFinalColor;
}
