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
```cpp
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
