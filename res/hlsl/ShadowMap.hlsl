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
};

cbuffer cb1 : register (b1)
{
    matrix g_matShadow1; // 첫 번째 그림자 맵의 변환 행렬
    matrix g_matShadow2; // 두 번째 그림자 맵의 변환 행렬
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
    float4 c : COLOR0; // 정점 색상
    float2 t : TEXCOORD0; // 텍스처 좌표
    float4 TexShadow1 : TEXCOORD1; // 첫 번째 그림자 맵의 텍스처 좌표
    float4 TexShadow2 : TEXCOORD2; // 두 번째 그림자 맵의 텍스처 좌표
};

// 정점 셰이더 함수
VS_OUTPUT VS(VS_INPUT vIn)
{
    VS_OUTPUT vOut = (VS_OUTPUT)0;
    // 월드 행렬을 통해 정점 위치를 변환
    vOut.p = mul(vIn.p, g_matWorld);
    // 뷰 행렬과 프로젝션 행렬을 거쳐 화면상의 위치로 변환
    vOut.p = mul(vOut.p, g_matView);
    vOut.p = mul(vOut.p, g_matProj);
    // 텍스처 좌표와 색상은 그대로 복사
    vOut.t = vIn.t;
    vOut.c = vIn.c;
    // 각 그림자 맵의 텍스처 좌표를 계산
    vOut.TexShadow1 = mul(vIn.p, g_matShadow1);
    vOut.TexShadow2 = mul(vIn.p, g_matShadow2);
    return vOut;
}


// 픽셀 셰이더 함수
float4 PS(VS_OUTPUT vIn) : SV_Target
{
    // 그림자 맵을 샘플링하여 그림자 존재 여부를 판별
    float4 shadow1 = g_txShadow1.Sample(g_samShadowMap, vIn.TexShadow1.xy / vIn.TexShadow1.w);
    float4 shadow2 = g_txShadow2.Sample(g_samShadowMap, vIn.TexShadow2.xy / vIn.TexShadow2.w);
    // 기본 텍스처의 색상을 샘플링
    float4 FinalColor = g_txDiffuse.Sample(g_SamLinear, vIn.t);

    // 그림자가 존재하는 경우 색상을 어둡게 처리
    if (shadow1.r > 0.01f || shadow2.r > 0.01f)
    {
        FinalColor = FinalColor * float4(0.5f, 0.5f, 0.5f, 1.0f);
        FinalColor.w = 1.0f;
    }

    // 최종적으로 색상을 0.3배로 줄임 (어두운 색조를 주기 위함)
    FinalColor *= 0.3f;
    FinalColor.w = 1.0f;
    return FinalColor;
}
