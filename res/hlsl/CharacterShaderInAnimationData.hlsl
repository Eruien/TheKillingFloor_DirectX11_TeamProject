#define MAX_BONE_MATRICES 100 
#define MAX_INSTANCING_MATRICES 100 
#define g_iNumLight  1

Texture2D g_txDiffuse1 : register(t0);
Texture2D<float4> g_txAnimationArray : register(t2);
SamplerState sample0 : register(s0);

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
    float4 v : TEXCOORD3; // for light
};

// 상수버퍼(레지스터 단위로 저장되어야 한다.)
// 레지스터 단위란, float4(x,y,z,w) 
cbuffer cb0 : register (b0)
{
    matrix g_matWorld  : packoffset(c0);// float4x4 // 4개    
    matrix g_matView  : packoffset(c4);
    matrix g_matProj  : packoffset(c8);
};

//cbuffer cbAnimMatrices : register (b1)
//{
//    float4x4 g_matBoneWorld[MAX_BONE_MATRICES];
//};

cbuffer cbCurrentFrame : register (b2)
{
    int g_CurrentFrame[4];
};

// for light
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

//
float4x4 ReadMatrix(int x, int y) {
    float4x4 mat;
    mat[0] = g_txAnimationArray.Load(int3(x * 4, y, 0));
    mat[1] = g_txAnimationArray.Load(int3(x * 4 + 1, y, 0));
    mat[2] = g_txAnimationArray.Load(int3(x * 4 + 2, y, 0));
    mat[3] = g_txAnimationArray.Load(int3(x * 4 + 3, y, 0));
    return mat;
}

VS_OUTPUT VS(VS_INPUT vIn)
{
    VS_OUTPUT vOut = (VS_OUTPUT)0;
  
    /*for (int i = 0; i < MAX_INSTANCING_MATRICES; i++)
    {
        g_matBoneWorld[i] = ReadMatrix(i, g_CurrentFrame[0]);
    }*/

    float4 vLocal = float4(vIn.p.xyz, 1.0f);
    float4 vWorld = 0;
    float3 vNormal = 0;

 /*   for (int iBone = 0; iBone < 4; iBone++)
    {
        uint iBoneIndex = vIn.i[iBone];
        vWorld += mul(vLocal, g_matBoneWorld[iBoneIndex]) * vIn.w[iBone];
        vNormal += mul(vIn.n, (float3x3)g_matBoneWorld[iBoneIndex]) * vIn.w[iBone];
    }*/

    for (int iBone = 0; iBone < 4; iBone++)
    {
        uint iBoneIndex = vIn.i[iBone];
        vWorld += mul(vLocal, ReadMatrix(iBoneIndex, g_CurrentFrame[0])) * vIn.w[iBone];
        vNormal += mul(vIn.n, (float3x3)ReadMatrix(iBoneIndex, g_CurrentFrame[0])) * vIn.w[iBone];
    }

    vWorld = mul(vWorld, g_matWorld);
    float4 vView = mul(vWorld, g_matView);
    float4 vProj = mul(vView, g_matProj);
    vOut.p = vProj;
    vOut.n = vNormal;
    vOut.t = vIn.t;
    vOut.c = vIn.c;
    vOut.v = vWorld; // for light
    return vOut;
}


float4 ComputePointLight(float3 vVertexPos, float3 vVertexNormal, int nNumLights)
{
    float4 vPointLightColor = float4(0, 0, 0, 1);
    for (int iLight = 0; iLight < nNumLights; iLight++)
    {
        float4 vLight;
        vLight.xyz = normalize(vVertexPos - g_vLightPos[iLight].xyz);
        vLight.w = distance(vVertexPos, g_vLightPos[iLight].xyz) / 4;

        float fAttenuation = 1.0f / (1.0f + 0.005f * vLight.w * vLight.w); // 감소함수 추가
        float fLuminance = smoothstep(vLight.w - 5, vLight.w, g_vLightPos[iLight].w);
        float fIntensity = saturate(dot(vVertexNormal, -vLight.xyz));

        vPointLightColor += float4(g_cDiffuseLightColor[iLight].rgb * fLuminance * fAttenuation, 1.0f); // 감소함수 적용
    }
    return vPointLightColor;
}


struct PS_IN
{
    float4 p : SV_POSITION;
    float2 t : TEXTURE;
};
float4 Diffuse(float3 vNormal)
{
    float3 vLightDir = { 0, -1, 0 };
    float fIntensity = max(0, dot(vNormal, -vLightDir));
    float4 diffuse = g_cAmbientMaterial[0] * g_cAmbientLightColor[0] +
		(g_cDiffuseMaterial[0] * g_cDiffuseLightColor[0] * fIntensity);
    return diffuse;
}

float4 PS(VS_OUTPUT vIn) : SV_Target
{
    //float depthValue = vIn.p.z / vIn.p.w;

    //            r,g,b,a(1)=불투명, a(0)=완전투명, a(0.0< 1.0f)= 반투명
    float4 vTexColor = g_txDiffuse1.Sample(sample0, vIn.t);
    float4 vPointLightColor = ComputePointLight(vIn.v, vIn.n, g_iNumLight);
    float4 vDiffuse = Diffuse(vIn.n);
    float4 FinalColor = vTexColor * (vPointLightColor + vDiffuse);
    
    float grayScale = dot(FinalColor.rgb, float3(0.299, 0.587, 0.114));
    float3 grayColor = float3(grayScale, grayScale, grayScale);
    FinalColor.rgb = lerp(FinalColor.rgb, grayColor, g_bIsZedTime);

    
    return FinalColor;
//return vIn.c;
}

float4 PS_Color(VS_OUTPUT vIn) : SV_Target
{
    float4 vFinalColor = ComputePointLight(vIn.v, vIn.n, g_iNumLight);
    return vFinalColor;
}