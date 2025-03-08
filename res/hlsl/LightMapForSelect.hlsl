#define WIDEN(x) g_##x
#define g_iNumLight  1
//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
// 텍스처와 샘플러 상태를 쉐이더에서 사용할 수 있도록 레지스터에 등록
Texture2D g_txDiffuse : register (t0); // 기본 텍스처
Texture2D g_txShadow1 : register (t1); // 첫 번째 그림자 맵
Texture2D g_txShadow2 : register (t2); // 두 번째 그림자 맵
SamplerState g_SamLinear : register (s0); // 기본 샘플러
SamplerState g_samShadowMap : register (s1); // 그림자 맵용 샘플러

// 상수 버퍼를 정의하고 패킹 오프셋을 사용하여 변수를 cbuffer의 특정 위치에 할당
cbuffer cb0 : register (b0)
{
    matrix g_matWorld  : packoffset(c0); // 월드 행렬
    matrix g_matView  : packoffset(c4); // 뷰 행렬
    matrix g_matProj  : packoffset(c8); // 프로젝션 행렬
    float4 g_MeshColor : packoffset(c12);
};

cbuffer cb1 : register (b1)
{
    matrix g_matShadow1; // 첫 번째 그림자 맵의 변환 행렬
    matrix g_matShadow2; // 두 번째 그림자 맵의 변환 행렬
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
// 정점 셰이더 입력 및 출력 구조체 정의
// 이 구조체들은 정점 셰이더의 입력 및 출력 데이터를 관리
struct VS_INPUT
{
    float4 p : POSITION; // 정점 위치
    float3 n : NORMAL; // 정점의 노멀 벡터
    float4 c : COLOR0; // 정점 색상
    float2 t : TEXCOORD0; // 텍스처 좌표
};

struct VS_OUTPUT
{
    float4 p : SV_POSITION; // 화면상의 정점 위치
    float3 n : NORMAL;
    float4 c : COLOR0; // 정점 색상
    float2 t : TEXCOORD0; // 텍스처 좌표
    float4 TexShadow1 : TEXCOORD1; // 첫 번째 그림자 맵의 텍스처 좌표
    float4 TexShadow2 : TEXCOORD2; // 두 번째 그림자 맵의 텍스처 좌표
    float4 v : TEXCOORD3;
};

// 정점 셰이더 함수
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

        //float fAttenuation = 1.0f / (1.0f + 0.005f * vLight.w * vLight.w); // 감소함수 추가
        float fLuminance = smoothstep(vLight.w - 5, vLight.w, g_vLightPos[iLight].w);
        float fIntensity = saturate(dot(vVertexNormal, -vLight.xyz));

        vPointLightColor += float4(g_cDiffuseLightColor[iLight].rgb * fLuminance, 1.0f); // 감소함수 적용
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


// 픽셀 셰이더 함수
float4 PS(VS_OUTPUT vIn) : SV_Target
{
    float4 vTexColor = g_txDiffuse.Sample(g_SamLinear, vIn.t);
    // 그림자 맵을 샘플링하여 그림자 존재 여부를 판별
    float4 shadow1 = g_txShadow1.Sample(g_samShadowMap, vIn.TexShadow1.xy / vIn.TexShadow1.w);
    float4 shadow2 = g_txShadow2.Sample(g_samShadowMap, vIn.TexShadow2.xy / vIn.TexShadow2.w);
    // light
    float4 vPointLightColor = ComputePointLight(vIn.v, vIn.n, g_iNumLight);
    // 기본 텍스처의 색상을 샘플링
    float4 vDiffuse = Diffuse(vIn.n);
    float4 FinalColor = vTexColor * (vPointLightColor + vDiffuse);

    

    // 그림자가 존재하는 경우 색상을 어둡게 처리
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
