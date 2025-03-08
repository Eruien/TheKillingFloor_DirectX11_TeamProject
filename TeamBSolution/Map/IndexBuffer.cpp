#include "IndexBuffer.h"
#include "Utils.h"


void IndexBuffer::CreateIndexBuffer(const std::vector<UINT>& indices)
{
	stride = sizeof(UINT);
	indexCount = indices.size();

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = indices.data();

	HRESULT hr = LGlobal::g_pDevice->CreateBuffer(&indexBufferDesc, &indexData, indexBuffer.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		Utils::ShowErrorMessage(hr);
	}
		
}

void IndexBuffer::PushData()
{
	LGlobal::g_pImmediateContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}
