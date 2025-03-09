# TheKillingFloor
* 플레이 영상 : <https://www.youtube.com/watch?v=5SlkLLaxNSU>
* 기술 소개 영상 : <https://www.youtube.com/watch?v=CBATlZ7Ium8>
* 다운로드 : <http://naver.me/Gal3Aupb>
***
* 작업 기간 : 2024. 01. 01 ~ 2024. 03. 25 (2개월)
* 인력 구성 : 3명
* 담당 파트 : 팀장(캐릭터)
* 사용언어 및 개발환경 : DirectX11, C++, FMOD
# 조작법    
* W, A, S, D : 이동
* F : 스팀팩
* B : 상점
* 1, 2 : 카메라 시점 전환
* 4, 5, 6 : 총기전환(상점에서 구입시 전환가능)
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
