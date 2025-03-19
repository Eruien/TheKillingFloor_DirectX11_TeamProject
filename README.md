# TheKillingFloor
* 플레이 영상 : <https://www.youtube.com/watch?v=5SlkLLaxNSU>
* 기술 소개 영상 : <https://www.youtube.com/watch?v=CBATlZ7Ium8>
* 다운로드 : <http://naver.me/Gal3Aupb>
***
* 작업 기간 : 2024. 01. 01 ~ 2024. 03. 25 (3달)
* 인력 구성 : 3명
* 담당 파트 : 팀장(캐릭터)
* 사용언어 및 개발환경 : DirectX11, C++, FMOD
# 조작법    
* W, A, S, D : 이동
* F : 스팀팩
* B : 상점
* 1, 2 : 카메라 시점 전환
* 4, 5, 6 : 총기 전환(상점에서 구입시 전환 가능)
* 마우스 좌클릭 : 공격
* 마우스 우클릭 : 줌 인
# Graphics Pipeline
![Pipeline Image](https://github.com/Eruien/TheKillingFloor_DirectX11_TeamProject/blob/main/Image/Pipeline.png)
# Pipeline Class
![Pipeline Class](https://github.com/Eruien/TheKillingFloor_DirectX11_TeamProject/blob/main/Image/PipelineClass.png)
# LDXObject
* 파이프라인을 사용하는 클래스들이 상속받을 최상위 부모 클래스
<details>
<summary> LDXObject 헤더파일</summary>
	
```cpp
// 그래픽 파이프 라인과 관련된 버퍼
// 파이프 라인에 넣어줄 정점 배열(m_VertexList)
// m_VertexList 배열에 SimpleVertex는 PNCT(Position, Normal, Color, Texture) 데이터로 구성

class LDXObject
{
public:
	ComPtr<ID3D11Device> m_pDevice;
	ComPtr<ID3D11DeviceContext> m_pImmediateContext;
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;
	ComPtr<ID3D11Buffer> m_pConstantBuffer;
	ComPtr<ID3D11InputLayout> m_pVertexLayout;
	std::vector<SimpleVertex> m_VertexList;
	std::vector<DWORD> m_IndexList;
	LTexture* m_Tex = nullptr;
	LShader* m_Shader = nullptr;
	CB_Data m_cbData;
public:
	void Set();
	virtual bool CreateVertexData();
	virtual bool CreateIndexData();
	virtual bool CreateVertexBuffer();
	virtual bool CreateIndexBuffer();
	virtual bool CreateConstantBuffer();
	virtual bool CreateLayout();
	virtual bool Create(std::wstring shaderFileName, std::wstring texFileName);
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool PreRender();
	virtual bool Render();
	virtual bool PostRender();
	virtual bool Release();
public:
	virtual ~LDXObject() {}
};
```

</details>

<details>
<summary> LDXObject 소스파일</summary>
	
```cpp
// 정점 구조체(SimpleVertex) * 구조체 개수 만큼 메모리 할당
// initialData에는 m_VertexList 배열의 처음 주소 할당
// IndexBuffer, ConstantBuffer도 비슷한 방식으로 생성
bool LDXObject::CreateVertexBuffer()
{    
    D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = sizeof(SimpleVertex) * m_VertexList.size();
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initialData;
	ZeroMemory(&initialData, sizeof(initialData));
	initialData.pSysMem = &m_VertexList.at(0);

	HRESULT hr = m_pDevice->CreateBuffer(
		&bufferDesc,
		&initialData,
		m_pVertexBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Create VertexBuffer Error", "Error Box", MB_OK);
		return false;
	}

	return true;
}

bool LDXObject::CreateIndexBuffer()
{
	if (m_IndexList.size() == 0) return true;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = sizeof(DWORD) * m_IndexList.size();
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA initialData;
	ZeroMemory(&initialData, sizeof(initialData));
	initialData.pSysMem = &m_IndexList.at(0);

	HRESULT hr = m_pDevice->CreateBuffer(
		&bufferDesc,
		&initialData,
		m_pIndexBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Create IndexBuffer Error", "Error Box", MB_OK);
		return false;
	}

	return true;
}

bool LDXObject::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC bufferDesc{};
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.ByteWidth = sizeof(CB_Data);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	HRESULT hr = m_pDevice->CreateBuffer(
		&bufferDesc,
		nullptr,
		m_pConstantBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Create ConstantBuffer Error", "Error Box", MB_OK);
		return false;
	}

	return true;
}

// 파이프 라인 정점(Vertex) 레이아웃(Layout) 설정
// 현재 프로젝트에서는 PNCT(Position, Normal, Color, Texture) 데이터를 사용
bool LDXObject::CreateLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	HRESULT hr = m_pDevice->CreateInputLayout(
		layout, sizeof(layout) / sizeof(layout[0]),
		m_Shader->m_pVSBlob->GetBufferPointer(),
		m_Shader->m_pVSBlob->GetBufferSize(),
		m_pVertexLayout.GetAddressOf());

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Create Input Layout Error", "Error Box", MB_OK);
		return false;
	}

	return true;
}

// 렌더링 작업전에 사전 작업처리 PreRender 함수
bool LDXObject::PreRender()
{
	m_pImmediateContext->IASetInputLayout(m_pVertexLayout.Get());
	m_pImmediateContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());

	if (m_Shader)
	{
		// VS와PS Shader를 stage에 세팅
		m_Shader->Apply();
	}

	if (m_Tex)
	{
		// PSSetShaderResources // ps에 m_pTexSRV 세팅
		m_Tex->Apply();
	}

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	return true;
}

// 렌더링 작업 함수
bool LDXObject::Render()
{
	PreRender();
	PostRender();
	return true;
}

// 렌더링 이후 작업 PostRender 함수
bool LDXObject::PostRender()
{
	if (m_pIndexBuffer == nullptr)
	{
		m_pImmediateContext->Draw(m_VertexList.size(), 0);
	}
	else
	{
		
		m_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		m_pImmediateContext->DrawIndexed(m_IndexList.size(), 0, 0);
	}
	
	return true;
}
```

</details>

# RenderingCode Flow
* 렌더링 코드 전체 흐름
![RenderingFlow](https://github.com/Eruien/TheKillingFloor_DirectX11_TeamProject/blob/main/Image/RenderingFlow.png)
# HLSL(High-Level Shader Language)
* 셰이더 코드 소개
<details>
<summary> HLSL 코드</summary>
	
```cpp
// ShaderResourceView에서 Diffuse Texture를 읽어 GPU 레지스터에 등록
Texture2D g_txDiffuse1 : register(t0);
// 필터링 제어에 사용
SamplerState sample0 : register(s0);

// 정점(Vertex) 데이터를 입력받을 때는 사용자가 설정한 레이아웃(Layout)의 이름(POSITION, NORMAL, COLOR, TEXCOORD)과 같아야 한다.
struct VS_INPUT
{
    float3 p : POSITION;
    float3 n : NORMAL;
    float4 c : COLOR;
    float2 t : TEXCOORD;
};

// 정점 데이터를 계산해서 이후의 파이프라인에 넘겨주게 되는데 위치(SV_POSITION)의 경우 고정된 이름으로 넘겨주게 된다. 어떠한 것이 위치인지 알아야 이후의 파이프라인에서 위치를 기반해서 처리하기 때문이다.
struct VS_OUTPUT
{
    float4 p : SV_POSITION;
    float3 n : NORMAL;
    float4 c : COLOR0;
    float2 t : TEXCOORD0;
};

// 상수버퍼(레지스터 단위로 저장되어야 한다.)
// 레지스터 단위란, float4(x,y,z,w)
cbuffer cb0
{
    matrix g_matWorld  : packoffset(c0);// float4x4 // 4개
    matrix g_matView  : packoffset(c4);
    matrix g_matProj  : packoffset(c8);
};

// 이후의 파이프라인에 넘겨주기 위해 정점(Vertex) 하나마다 행렬을 곱해준다.
VS_OUTPUT VS(VS_INPUT vIn)
{
    VS_OUTPUT vOut = (VS_OUTPUT)0;
    float4 vWorld = mul(float4(vIn.p,1.0f),g_matWorld);
    float4 vView = mul(vWorld, g_matView);
    float4 vProj = mul(vView, g_matProj);
    vOut.p = vProj;
    vOut.n = vIn.n;
    vOut.t = vIn.t;
    vOut.c = vIn.c;
    return vOut;
}

// 위에까지 VertexShader
// 아래부터 PixelShader

// SV_Target : 반환 값의 의도가 색상이라는 걸 나타낸다.
// 텍스처(Texture)에서 Sampler State로 필터링 된 색상을 반환 
float4 PS(VS_OUTPUT vIn) : SV_Target
{
    // r,g,b,a(1)=불투명, a(0)=완전투명, a(0.0< 1.0f)= 반투명
    return g_txDiffuse1.Sample(sample0, vIn.t);
}
```

</details>

# Sword Trail
* 칼이 휘두르는 궤적에 따라 정점을 생성하여 시각화
![SwordTrail_1](https://github.com/Eruien/TheKillingFloor_DirectX11_TeamProject/blob/main/Image/SwordTrail_1.png)
![SwordTrail_2](https://github.com/Eruien/TheKillingFloor_DirectX11_TeamProject/blob/main/Image/SwordTrail_2.png)

<details>
<summary> Sword Trail 헤더파일</summary>
	
```cpp
class LTrail : public LObject
{
public:
        // 트레일에 사용될 정점의 개수
	int m_iTrailVertexSize = 96;
	int m_iTrailCountSize = 95;
        // 0 ~ 95까지 시간이 지나면 카운트 증가
	int m_iVertexCount = 0;
        // 트레일의 시작 타이머값을 0으로 설정할 시 초반 트레일이 사라지는 문제가 있어서 End값보다 크게 설정
	float m_TimerStart = 1.0;
	float m_TimerEnd = 0.01;
        // Catmull-Rom Splines에 사용될 배열
	int m_iCatmullRomIndex[4] = { 0, };
public:
	void RenderTrail(TVector3* localSwordLow, TVector3* localSwordHigh, TMatrix* matSwordParent);
	void InterpolRenderTrail(TVector3* localSwordLow, TVector3* localSwordHigh, TMatrix* matSwordParent);
public:
	bool CreateVertexData() override;
	bool CreateIndexData() override;
};
```

</details>
<details>
<summary> Sword Trail 소스파일</summary>
	
```cpp
// m_TimerEnd의 경우 하나의 텍스처(Texture) 트레일(Trail)이 끝나는 시간입니다. 시간이 경과되면 타이머를 초기화하고 다음 텍스처(Texture)를 그리기 시작합니다.
// m_iVertexCount는 정점 개수의 최대치입니다. 최대치를 넘어가면 전체 정점을 배열에서 제거하고 화면에서 트레일 텍스처(Textrue)를 지웁니다.
// 이후에는 Catmull-Rom Splines에 사용할 정점 4개를 선정하는  과정입니다.
void LTrail::InterpolRenderTrail(TVector3* localSwordLow, TVector3* localSwordHigh, TMatrix* matSwordParent)
{
    m_TimerStart += LGlobal::g_fSPF;

    if (m_TimerStart > m_TimerEnd)
    {
        m_TimerStart = 0.0f;
        m_iVertexCount += 2;
    }

    if (m_iVertexCount > m_iTrailCountSize)
    {
        m_VertexList.clear();
        m_VertexList.resize(m_iTrailVertexSize);
        m_iVertexCount = 0;
    }

    if (m_iVertexCount - 2 < 0)
    {
        m_iCatmullRomIndex[0] = m_iTrailCountSize - 1;
    }
    else
    {
        m_iCatmullRomIndex[0] = m_iVertexCount - 2;
    }

    m_iCatmullRomIndex[1] = m_iVertexCount;

    if (m_iVertexCount + 2 > m_iTrailCountSize)
    {
        m_iCatmullRomIndex[2] = 0;
        m_iCatmullRomIndex[3] = 2;
    }
    else
    {
        m_iCatmullRomIndex[2] = m_iVertexCount + 2;
        m_iCatmullRomIndex[3] = m_iVertexCount + 4;
    }

    if (m_iVertexCount + 4 > m_iTrailCountSize)
    {
        m_iCatmullRomIndex[3] = 0;
    }
    else
    {
        m_iCatmullRomIndex[3] = m_iVertexCount + 4;
    }

    for (int i = 0; i < 1; i++)
    {
        D3DXVec3CatmullRom(&m_VertexList[m_iVertexCount].p,
            &m_VertexList[m_iCatmullRomIndex[0]].p,
            &m_VertexList[m_iCatmullRomIndex[1]].p,
            &m_VertexList[m_iCatmullRomIndex[2]].p,
            &m_VertexList[m_iCatmullRomIndex[3]].p,
            0.5);

        D3DXVec3CatmullRom(&m_VertexList[m_iVertexCount + 1].p,
            &m_VertexList[m_iCatmullRomIndex[0] + 1].p,
            &m_VertexList[m_iCatmullRomIndex[1] + 1].p,
            &m_VertexList[m_iCatmullRomIndex[2] + 1].p,
            &m_VertexList[m_iCatmullRomIndex[3] + 1].p,
            0.5);
    }

    D3DXVec3TransformCoord(&m_VertexList[m_iVertexCount].p, localSwordLow, matSwordParent);
    D3DXVec3TransformCoord(&m_VertexList[m_iVertexCount + 1].p, localSwordHigh, matSwordParent);

    for (int i = 0; i < m_iVertexCount; i += 2)
    {
        m_VertexList[i].t = { float(i) / (float(m_iVertexCount) - 2), 0.0f };
        m_VertexList[i + 1].t = { float(i) / (float(m_iVertexCount) - 2), 1.0f };
    }

    LGlobal::g_pImmediateContext->UpdateSubresource(m_pVertexBuffer.Get(), 0, NULL, m_VertexList.data(), 0, 0);
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    LGlobal::g_pImmediateContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
    SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    Render();
}

bool LTrail::CreateVertexData()
{
	// 업데이트시 pnct값을 넣어주기 때문에 구조체에 값을 넣어주지 않아도 됨
	m_VertexList.resize(m_iTrailVertexSize);
	
	return true;
}

bool LTrail::CreateIndexData()
{
	int indexVertexSize = (m_iTrailVertexSize - 2) * 3;
	m_IndexList.resize(indexVertexSize);

	int squareSize = (indexVertexSize / 6) * 2;

	for (int i = 0; i < squareSize; i += 2)
	{
		m_IndexList[(i * 3)] = i + 3;
		m_IndexList[(i * 3) + 1] = i + 1;
		m_IndexList[(i * 3) + 2] = i;

		m_IndexList[(i * 3) + 3] = i + 2;
		m_IndexList[(i * 3) + 4] = i + 3;
		m_IndexList[(i * 3) + 5] = i;
	}

	return true;
}


```

</details>

# FSM(finite-state-machine)
* 유한한 상태를 정해놓고 사용하는 설계 기법
![FSM_1](https://github.com/Eruien/TheKillingFloor_DirectX11_TeamProject/blob/main/Image/FSM_1.png)
![FSM_2](https://github.com/Eruien/TheKillingFloor_DirectX11_TeamProject/blob/main/Image/FSM_2.png)
![FSM_3](https://github.com/Eruien/TheKillingFloor_DirectX11_TeamProject/blob/main/Image/FSM_3.png)

<details>
<summary> FSM 헤더파일</summary>
	
```cpp
// Map에는 현재 상태가 키(Key)로 저장되고 현재 상태가 어떤 FSM State 객체에 속하는지가 값(Value)으로 저장
class LFiniteStateMachine
{
public:
	std::map<State, std::unique_ptr<LFiniteState>> m_FiniteStateMap;
public:
	void AddStateTransition(State keyState, Event inputEvent, State outputState);
	State StateTransition(State currentState, Event inputEvent);
};

// 현재 상태를 저장하고 현재상태에서 어떠한 이벤트가 발생해야 다음 상태로 갈지 맵에 키와 값으로  저장
class LFiniteState
{
private:
	State m_State;
public:
	std::map<Event, State> m_StateMap;
public:
	State GetState() { return m_State; }
public:
	void AddTransition(Event inputEvent, State outputState);
	State Output(Event inputEvent);
public:
	LFiniteState(State state);
};
```

</details>
<details>
<summary> FSM 소스파일</summary>
	
```cpp
// Map에 현재 상태가 등록되어 있지 않다면 등록
// 현재 상태에서 매개변수 이벤트가 발생할 시 다음 상태로 변환되도록 Map에 저장
void LFiniteStateMachine::AddStateTransition(State keyState, Event inputEvent, State outputState)
{
	auto finiteIter = m_FiniteStateMap.find(keyState);

	if (m_FiniteStateMap.end() != finiteIter)
	{
		finiteIter->second->AddTransition(inputEvent, outputState);
	}
	else
	{
		m_FiniteStateMap.insert(std::make_pair(keyState, std::make_unique<LFiniteState>(keyState)));

		finiteIter = m_FiniteStateMap.find(keyState);
		finiteIter->second->AddTransition(inputEvent, outputState);
	}
}

// 현재 상태에서 해당되는 이벤트가 있다면 다음 이벤트를 반환
// 해당 상태가 등록되어 있지 않은 경우 None을 반환
State LFiniteStateMachine::StateTransition(State currentState, Event inputEvent)
{
	auto finiteIter = m_FiniteStateMap.find(currentState);

	if (finiteIter == m_FiniteStateMap.end())
	{
		return State::NONE;
	}

	auto retStateIter = finiteIter->second->m_StateMap.find(inputEvent);

	if (retStateIter == finiteIter->second->m_StateMap.end())
	{
		return State::NONE;
	}

	State state = retStateIter->second;
	return state;
}

// 현재상태에서 어떠한 이벤트가 발생해야 다음 상태로 갈지 맵에 키와 값으로 저장
void LFiniteState::AddTransition(Event inputEvent, State outputState)
{
	m_StateMap.insert(std::make_pair(inputEvent, outputState));
}

// 이벤트를 키값으로 어떠한 상태로 변환될지 검색
State LFiniteState::Output(Event inputEvent)
{
	auto iter = m_StateMap.find(inputEvent);
	State state = iter->second;
	return state;
}
```

</details>

# FBX Loader
* FBX 노드의 메쉬랑 본 데이터를 읽어서 프로젝트에서 사용할 수 있게 컨버팅
![FBXLoader_1](https://github.com/Eruien/TheKillingFloor_DirectX11_TeamProject/blob/main/Image/FBXLoader_1.png)
![FBXLoader_2](https://github.com/Eruien/TheKillingFloor_DirectX11_TeamProject/blob/main/Image/FBXLoader_2.png)

<details>
<summary> FBX Loader 헤더파일</summary>
	
```cpp
// FbxScene에서 RootNode를 가져와서 저장
// 노드는 빛, 카메라, 본, 메쉬등 여러 가지로 이루어져 있는데 프로젝트에서는 본과 메쉬를 사용
// m_MeshNodeList : 메쉬 노드 저장
// m_pNodeList : 본 노드를 저장
class LFbxImport
{
	FbxManager* m_pFbxManager = nullptr;
	FbxImporter* m_pFbxImporter = nullptr;
	FbxScene* m_pFbxScene = nullptr;
	FbxNode* m_RootNode = nullptr;
	std::vector<std::shared_ptr<LFbxObj>> m_MeshNodeList;
	std::vector<FbxNode*> m_pNodeList;
	std::map<FbxNode*, int> m_pFbxNodeNameMap;
public:
	void GetAnimation(LFbxObj* fbxObj);
public:
	FbxVector2 ReadTextureCoord(FbxLayerElementUV* layer, int iVertexIndex, int iIndex);
	FbxColor ReadColor(FbxLayerElementVertexColor* layer, int iVertexIndex, int iIndex);
	FbxVector4 ReadNormal(FbxLayerElementNormal* layer, int iVertexIndex, int iIndex);
	std::string ParseMaterial(FbxSurfaceMaterial* pMaterial);
	int GetSubMaterialIndex(FbxLayerElementMaterial* layer, int iPoly);
public:
	TMatrix ConvertAMatrix(FbxMatrix& m);
	TMatrix ConvertAMatrix(FbxAMatrix& m);
	TMatrix DxConvertMatrix(TMatrix& m);
	TMatrix ParseTransform(FbxNode* fbxNode);
public:
	bool ParseMeshSkinning(FbxMesh* pFbxMesh, LFbxObj* pMesh);
public:
	bool Load(std::wstring fbxFilePath, LFbxObj* fbxObj);
	void PreProcess(FbxNode* fbxNode, LFbxObj* fbxObj);
	void MeshLoad(FbxNode* fbxNode, LFbxObj* lMesh);
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};
```

</details>
<details>
<summary> FBX Loader 소스파일</summary>
	
```cpp
// 함수는 여러가지가 있지만 전체적으로 FBX 메쉬의 정점을 가지고 와서 MAX -> DirectX11로 사용할 수 있게 컨버팅 하는 로직

인자로 넣어준 fbxObject에게 애니메이션 정보를 세팅해준다.
void LFbxImport::GetAnimation(LFbxObj* fbxObj)
{
        // 애니메이션 60프레임으로 설정
	FbxTime::SetGlobalTimeMode(FbxTime::eFrames60);
        // Scene에서 애니메이션 정보를 가져온다.
	FbxAnimStack* stack = m_pFbxScene->GetSrcObject<FbxAnimStack>(0);

	if (stack == nullptr)
	{
		MessageBoxA(NULL, "AnimationStack is Null", "Error Box", MB_OK);
		return;
	}

        이름을 가져와서 시작프레임과 끝 프레임을 가져온다.
	FbxString takeName = stack->GetName();
	FbxTakeInfo* takeInfo = m_pFbxScene->GetTakeInfo(takeName);
	FbxTimeSpan ts = takeInfo->mLocalTimeSpan;
	FbxTime startTime = ts.GetStart();
	FbxTime endTime = ts.GetStop();

        // 위에서 설정한 프레임 정보를 넣어준다.
	FbxTime::EMode timeMode = FbxTime::GetGlobalTimeMode();
	FbxLongLong startFrame = startTime.GetFrameCount(timeMode);
	FbxLongLong endFrame = endTime.GetFrameCount(timeMode);

        // 애니메이션 프레임 세팅 
	fbxObj->SetAnimation(startFrame, endFrame, 60, 160);

	FbxTime time;
        // 프레임 크기만큼 사이즈 조정
	fbxObj->m_NameMatrixMap.resize(endFrame);
	for (int t = startFrame; t < endFrame; t++)
	{
		time.SetFrame(t, timeMode);
		std::vector<TMatrix> matArray;
		for (int iMesh = 0; iMesh < fbxObj->m_TreeList.size(); iMesh++)
		{
			// 초당 60프레임 기준으로 프레임1당 fbxMatrix
			// Mesh당 행렬을 얻는다.
			FbxAMatrix fbxMat = m_pNodeList[iMesh]->EvaluateGlobalTransform(time);
			TMatrix mat = ConvertAMatrix(fbxMat);
			mat = DxConvertMatrix(mat);
			matArray.push_back(mat);

			std::wstring nodeName = mtw(m_pNodeList[iMesh]->GetName());
			fbxObj->m_NameMatrixMap[t].insert(std::make_pair(nodeName, mat));
		}
		fbxObj->m_MatrixArray.push_back(matArray);
	}
}

배열에서 텍스처 좌표를 읽는 함수
FbxVector2 LFbxImport::ReadTextureCoord(FbxLayerElementUV* layer, int iVertexIndex, int iIndex)
{
	FbxVector2 texture;

	switch (layer->GetMappingMode())
	{
	case FbxLayerElementUV::eByControlPoint:
	{
		switch (layer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		{
			texture = layer->GetDirectArray().GetAt(iVertexIndex);
		}
		break;

		case FbxLayerElementUV::eIndexToDirect:
		{
			int id = layer->GetIndexArray().GetAt(iVertexIndex);
			texture = layer->GetDirectArray().GetAt(id);
		}
		break;
		}
	}
	break;

	case FbxLayerElementUV::eByPolygonVertex:
	{
		switch (layer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		case FbxLayerElementUV::eIndexToDirect:
		{
			texture = layer->GetDirectArray().GetAt(iIndex);
		}
		break;
		}
	}
	break;

	}

	return texture;
}

배열에서 컬러값을 읽는 함수 
FbxColor LFbxImport::ReadColor(FbxLayerElementVertexColor* layer, int iVertexIndex, int iIndex)
{
	FbxColor color;

	switch (layer->GetMappingMode())
	{
	case FbxLayerElementVertexColor::eByControlPoint:
	{
		switch (layer->GetReferenceMode())
		{
		case FbxLayerElementVertexColor::eDirect:
		{
			color = layer->GetDirectArray().GetAt(iVertexIndex);
		}
		break;
		case FbxLayerElementVertexColor::eIndexToDirect:
		{
			int id = layer->GetIndexArray().GetAt(iVertexIndex);
			color = layer->GetDirectArray().GetAt(id);
		}
		break;
		}
	}
	break;
	case FbxLayerElementVertexColor::eByPolygonVertex:
	{
		switch (layer->GetReferenceMode())
		{
		case FbxLayerElementVertexColor::eDirect:
		{
			color = layer->GetDirectArray().GetAt(iIndex);
		}
		break;
		case FbxLayerElementVertexColor::eIndexToDirect:
		{
			int id = layer->GetIndexArray().GetAt(iIndex);
			color = layer->GetDirectArray().GetAt(id);
		}
		break;
		}
	}
	break;
	}

	return color;
}

// 배열에서 노말을 읽는 함수
FbxVector4 LFbxImport::ReadNormal(FbxLayerElementNormal* layer, int iVertexIndex, int iIndex)
{
	FbxVector4 normal;

	switch (layer->GetMappingMode())
	{
	case FbxLayerElementNormal::eByControlPoint:
	{
		switch (layer->GetReferenceMode())
		{
		case FbxLayerElementNormal::eDirect:
		{
			normal = layer->GetDirectArray().GetAt(iVertexIndex);
		}
		break;
		case FbxLayerElementNormal::eIndexToDirect:
		{
			int id = layer->GetIndexArray().GetAt(iVertexIndex);
			normal = layer->GetDirectArray().GetAt(id);
		}
		break;
		}
	}
	break;
	case FbxLayerElementNormal::eByPolygonVertex:
	{
		switch (layer->GetReferenceMode())
		{
		case FbxLayerElementNormal::eDirect:
		{
			normal = layer->GetDirectArray().GetAt(iIndex);
		}
		break;
		case FbxLayerElementNormal::eIndexToDirect:
		{
			int id = layer->GetIndexArray().GetAt(iIndex);
			normal = layer->GetDirectArray().GetAt(id);
		}
		break;
		}
	}
	break;
	}

	return normal;
}

머터리얼 파일의 경로를 읽어오는 함수
std::string LFbxImport::ParseMaterial(FbxSurfaceMaterial* pSurfaceMaterial)
{
	std::string materialFullPath;

	FbxProperty fbxProperty = pSurfaceMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);

	if (!fbxProperty.IsValid())
	{
		MessageBoxA(NULL, "FindProperty Error", "Error Box", MB_OK);

		return materialFullPath;
	}

	FbxFileTexture* texFile = fbxProperty.GetSrcObject<FbxFileTexture>(0);

	if (texFile != nullptr)
	{
		materialFullPath = texFile->GetFileName();

		return materialFullPath;
	}

	return materialFullPath;
}

서브 머터리얼의 경로를 읽어 오는 함수
int LFbxImport::GetSubMaterialIndex(FbxLayerElementMaterial* layer, int iPoly)
{
	int SubMaterialIndex = 0;

	switch (layer->GetMappingMode())
	{
	case FbxLayerElementMaterial::eByPolygon:
	{
		switch (layer->GetReferenceMode())
		{
			// eDirect 폐기됨 eDirect 말고 eByPolygon의 eIndexToDirect를 사용할것 해당 폴리곤을 넣으면 서브메터리얼이 바로 나옴
		case FbxLayerElementMaterial::eDirect:
		case FbxLayerElementMaterial::eIndexToDirect:
		{
			SubMaterialIndex = layer->GetIndexArray().GetAt(iPoly);
		}
		break;
		}
	}
	break;
	}

	return SubMaterialIndex;
}

FBX 행렬을 TMatrix로 변환
TMatrix LFbxImport::ConvertAMatrix(FbxMatrix& m)
{
	TMatrix mat;
	float* pMatArray = reinterpret_cast<float*>(&mat);
	double* pSrcArray = reinterpret_cast<double*>(&m);

	for (int i = 0; i < 16; i++)
	{
		pMatArray[i] = pSrcArray[i];
	}

	return mat;
}

TMatrix를 현재 쓰는 행렬로 변환
TMatrix LFbxImport::DxConvertMatrix(TMatrix& m)
{
	TMatrix mat;
	mat._11 = m._11; mat._12 = m._13; mat._13 = m._12;
	mat._21 = m._31; mat._22 = m._33; mat._23 = m._32;
	mat._31 = m._21; mat._32 = m._23; mat._33 = m._22;
	mat._41 = m._41; mat._42 = m._43; mat._43 = m._42;
	mat._14 = mat._24 = mat._34 = 0.0f; mat._44 = 1.0f;

	return mat;
}

FBXNode가 들어오면 바로 사용하는 행렬로 변환 되게 함수 정리
TMatrix LFbxImport::ParseTransform(FbxNode* fbxNode)
{
	FbxTime::EMode TimeMode = FbxTime::GetGlobalTimeMode();
	FbxTime time;
	time.SetFrame(0.0f, TimeMode);
	FbxAMatrix matFbx = fbxNode->EvaluateGlobalTransform(time);
	TMatrix mat = ConvertAMatrix(matFbx);
	mat = DxConvertMatrix(mat);

	return mat;
}

// FBXObject에서 불필요한걸 제외하고 메쉬랑 본을 뽑아내기 위한 작업
bool LFbxImport::Load(std::wstring fbxFilePath, LFbxObj* fbxObj)
{
	bool ret = m_pFbxImporter->Initialize(wtm(fbxFilePath).c_str());
	m_pFbxImporter->Import(m_pFbxScene);

	if (!ret)
	{
		MessageBoxA(NULL, "FbxFile Import Error", "Error Box", MB_OK);
		return false;
	}

	m_RootNode = m_pFbxScene->GetRootNode();

	if (m_RootNode == nullptr)
	{
		MessageBoxA(NULL, "FbxRootNode is Null", "Error Box", MB_OK);
		return false;
	}
	
	PreProcess(m_RootNode, fbxObj);

	// 파일 이름만 빠진 경로를 뽑아냄 path에 저장되어 있음
	std::size_t found = fbxFilePath.find_last_of(L"/");
	std::wstring path = fbxFilePath.substr(0, found + 1);

	for (int i = 0; i < m_MeshNodeList.size(); i++)
	{
		std::shared_ptr<LFbxObj> fbxMesh = m_MeshNodeList[i];
		fbxMesh->m_DefaultFilePath = path;
		//auto iter = m_pFbxNodeNameMap.find(m_MeshNodeList[i]);
		// 메쉬가 있는 노드에서 메쉬 관련된 정보를 뽑아냄
		MeshLoad(m_pNodeList[fbxMesh->m_iBoneIndex], fbxMesh.get());
		
		fbxObj->m_DrawList.push_back(fbxMesh);
	}

	GetAnimation(fbxObj);

	return true;
}

// 루트씬에서 카메라 정보나 빛 정보를 빼고 메쉬나 본 필요한 정보만 사용
void LFbxImport::PreProcess(FbxNode* fbxNode, LFbxObj* fbxObj)
{
	if (fbxNode == nullptr) return;
	if (fbxNode->GetCamera() || fbxNode->GetLight()) return;

	std::shared_ptr<LFbxObj> fbxChildObj = std::make_shared<LFbxObj>();
	fbxChildObj->m_Name = mtw(fbxNode->GetName());
	fbxChildObj->m_iBoneType = LFbxObj::BONE_NODE;
	fbxChildObj->m_iBoneIndex = fbxObj->m_TreeList.size();

	fbxObj->m_TreeList.push_back(fbxChildObj);
	fbxObj->m_pFbxNodeNameList.push_back(fbxChildObj->m_Name);
	fbxObj->m_pFbxNodeNameMap.insert(std::make_pair(fbxChildObj->m_Name, fbxChildObj->m_iBoneIndex));

	m_pNodeList.push_back(fbxNode);
	m_pFbxNodeNameMap.insert(std::make_pair(fbxNode, fbxChildObj->m_iBoneIndex));
	
	if (fbxNode->GetMesh())
	{
		fbxChildObj->m_iBoneType = LFbxObj::MESH_NODE;
		m_MeshNodeList.push_back(fbxChildObj);
	}
	
	for (int i = 0; i < fbxNode->GetChildCount(); i++)
	{
		PreProcess(fbxNode->GetChild(i), fbxObj);
	}
}

// 메쉬를 뽑아내는 자세한 과정
void LFbxImport::MeshLoad(FbxNode* fbxNode, LFbxObj* lMesh)
{
	// 메쉬가 있는 노드에서 메쉬를 가지고 옴
	FbxMesh* fbxMesh = fbxNode->GetMesh();
	// 메쉬가 가지고 있는 폴리곤의 총 갯수
	int iPolyCount = fbxMesh->GetPolygonCount();

	lMesh->m_bSkinning = ParseMeshSkinning(fbxMesh, lMesh);
	lMesh->m_iBoneType = LFbxObj::SKIN_NODE;

	
	// 제어점의 배열을 가지고 옴
	FbxVector4* fbxControlPointList = fbxMesh->GetControlPoints();
	// 레이어들의 반복을 돌릴 레이어 카운터
	int iLayerCount = fbxMesh->GetLayerCount();
	// 메쉬가 가지고 있는 재질의 가짓수 사실상 이것대로 정점을 분할해서 넣을것이기 때문에 SubMaterial갯수라고 봐도 상관없다.
	int iSubMaterialCount = fbxNode->GetMaterialCount();
	// 서브메테리얼 인덱스가 담길 공간 기본값은 0으로 하나의 공간에 담긴다.
	int iSubMaterialIndex = 0;

	std::vector<FbxLayerElementNormal*> NormalLayerList;
	std::vector<FbxLayerElementVertexColor*> ColorLayerList;
	std::vector<FbxLayerElementUV*> UVLayerList;
	std::vector<FbxLayerElementMaterial*> MaterialLayerList;

	// Mesh가 가지고 있는 레이어 각각의 레이어 배열 뽑아내기
	for (int iLayer = 0; iLayer < iLayerCount; iLayer++)
	{
		FbxLayer* fbxLayer = fbxMesh->GetLayer(iLayer);

		if (fbxLayer->GetNormals() != nullptr)
		{
			NormalLayerList.push_back(fbxLayer->GetNormals());
		}

		if (fbxLayer->GetVertexColors() != nullptr)
		{
			ColorLayerList.push_back(fbxLayer->GetVertexColors());
		}

		if (fbxLayer->GetUVs() != nullptr)
		{
			UVLayerList.push_back(fbxLayer->GetUVs());
		}

		if (fbxLayer->GetMaterials() != nullptr)
		{
			MaterialLayerList.push_back(fbxLayer->GetMaterials());
		}
	}

	// Mesh의 재질이 여러개면 각각의 정점 리스트에 넣는다.
	if (iSubMaterialCount > 0)
	{
		lMesh->m_TriangleList.resize(iSubMaterialCount);
		lMesh->m_pSubVertexListIW.resize(iSubMaterialCount);
	}

	// 텍스처 파일의 이름을 불러오고 그걸 기반으로 텍스처 배열도 로드한다.
	for (int iSub = 0; iSub < iSubMaterialCount; iSub++)
	{
		std::string filePath;
		FbxSurfaceMaterial* surfaceMaterial = fbxNode->GetMaterial(iSub);
		filePath = ParseMaterial(surfaceMaterial);

		char Drive[MAX_PATH];
		char Dir[MAX_PATH];
		char FName[MAX_PATH];
		char Ext[MAX_PATH];

		_splitpath_s(filePath.c_str(), Drive, MAX_PATH, Dir, MAX_PATH, FName, MAX_PATH, Ext, MAX_PATH);

		std::string texName = FName;
		texName += Ext;

		lMesh->m_TexFileNameList.push_back(mtw(texName));
	}

	// 행렬 정보를 불러온다. 행렬 정보는 local정점 x 자신과부모의 변환행렬 x 기하학적 행렬
	// 자신과 부모의 변환행렬은 ParseTransform 에서 기하학적 행렬은 바로 아래에서 뽑아낸다.
	FbxAMatrix GeometircMatrix;
	FbxAMatrix normalMatrix;
	{
		FbxVector4 s = fbxNode->GetGeometricScaling(FbxNode::eSourcePivot);
		FbxVector4 r = fbxNode->GetGeometricRotation(FbxNode::eSourcePivot);
		FbxVector4 t = fbxNode->GetGeometricTranslation(FbxNode::eSourcePivot);

		GeometircMatrix.SetS(s);
		GeometircMatrix.SetR(r);
		GeometircMatrix.SetT(t);

		normalMatrix = GeometircMatrix;
		normalMatrix = normalMatrix.Inverse();
		normalMatrix = normalMatrix.Transpose();
	}

	// 제어점 기반이 아니라 폴리곤의 정점 기반 계산이다.
	// 삼각형 기반 cell이 9개일 경우 9 x 2 x 3 = 54개의 정점 계산
	// 폴리곤의 숫자, 삼각형의 숫자, 삼각형의 정점의 숫자
	int iBasePolyIndex = 0;
	for (int iPoly = 0; iPoly < iPolyCount; iPoly++)
	{
		int iPolySize = fbxMesh->GetPolygonSize(iPoly);
		int iTriangleCount = iPolySize - 2;
		// 폴리곤에서 해당 재질의 인덱스를 가져온다.
		iSubMaterialIndex = GetSubMaterialIndex(MaterialLayerList[0], iPoly);

		// 폴리곤 총 갯수의 정보를 저장
		lMesh->m_NumPoly += iPolySize;
		iBasePolyIndex += iPolySize;
		for (int iTriangle = 0; iTriangle < iTriangleCount; iTriangle++)
		{
			int CornerIndices[3];
			CornerIndices[0] = fbxMesh->GetPolygonVertex(iPoly, 0);
			CornerIndices[1] = fbxMesh->GetPolygonVertex(iPoly, iTriangle + 2);
			CornerIndices[2] = fbxMesh->GetPolygonVertex(iPoly, iTriangle + 1);

			int uv[3];
			uv[0] = fbxMesh->GetTextureUVIndex(iPoly, 0);
			uv[1] = fbxMesh->GetTextureUVIndex(iPoly, iTriangle + 2);
			uv[2] = fbxMesh->GetTextureUVIndex(iPoly, iTriangle + 1);

			int iPolyIndex[3] = { 0, iTriangle + 2, iTriangle + 1 };
		
			for (int iVertex = 0; iVertex < 3; iVertex++)
			{
				SimpleVertex pnct;

				int iDCIndex = CornerIndices[iVertex];
				FbxVector4 v = fbxControlPointList[iDCIndex];
				FbxVector4 finalPos = GeometircMatrix.MultT(v);
				pnct.p.x = finalPos.mData[0];
				pnct.p.y = finalPos.mData[2];
				pnct.p.z = finalPos.mData[1];

				if (NormalLayerList.size() > 0)
				{
					FbxVector4 normal = {0.0f, 0.0f, 0.0f};
					normal = ReadNormal(NormalLayerList[0], iDCIndex, uv[iVertex] + iBasePolyIndex);

					pnct.n.x = normal.mData[0];
					pnct.n.y = normal.mData[2];
					pnct.n.z = normal.mData[1];
				}

				if (ColorLayerList.size() > 0)
				{
					FbxColor color = { 1.0f, 1.0f, 1.0f, 1.0f };
					color = ReadColor(ColorLayerList[0], iDCIndex, uv[iVertex] + iBasePolyIndex);

					pnct.c.x = color.mRed;
					pnct.c.y = color.mGreen;
					pnct.c.z = color.mBlue;
					pnct.c.w = 1.0f;
				}

				FbxVector2 textureUV = ReadTextureCoord(UVLayerList[0], iDCIndex, uv[iVertex]);
				pnct.t.x = textureUV.mData[0];
				pnct.t.y = 1.0f - textureUV.mData[1];

				lMesh->m_TriangleList[iSubMaterialIndex].push_back(pnct);

				LVertexIW iwVertex;

				FbxGeometryElementTangent* vertexTangent = fbxMesh->GetElementTangent(0);
				iwVertex.tan = { 0, 0, 0 };

				if (lMesh->m_bSkinning)
				{
					LWeight* weight = &lMesh->m_WeightList[iDCIndex];

					for (int i = 0; i < 4; i++)
					{
						iwVertex.i[i] = weight->index[i];
						iwVertex.w[i] = weight->weight[i];
					}
				}
				else
				{
					iwVertex.i[0] = lMesh->m_iBoneIndex;
					iwVertex.w[0] = 1.0f;
				}
				lMesh->m_pSubVertexListIW[iSubMaterialIndex].push_back(iwVertex);
			}
		}
	}
}
```

</details>
