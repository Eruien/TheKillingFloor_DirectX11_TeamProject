#include "LManagerAgr.h"
#include "LGlobal.h"

//Texture
void LTexture::Apply()
{
	LGlobal::g_pImmediateContext->PSSetShaderResources(0, 1, &m_pTexSRV);
}

bool LTexture::Load(std::wstring fileName)
{
	auto imageobj = std::make_unique<DirectX::ScratchImage>();
	DirectX::TexMetadata mdata;
	m_texPath = fileName;
	HRESULT hr = DirectX::GetMetadataFromWICFile(fileName.c_str(), DirectX::WIC_FLAGS_NONE, mdata);
	hr = DirectX::LoadFromWICFile(fileName.c_str(), DirectX::WIC_FLAGS_NONE, &mdata, *imageobj);
	hr = DirectX::CreateShaderResourceView(LGlobal::g_pDevice.Get(), imageobj->GetImages(), imageobj->GetImageCount(), mdata, &m_pTexSRV);

	if (SUCCEEDED(hr))
	{
		size.x = mdata.width;
		size.y = mdata.height;
		return true;
	}

	if (FAILED(hr))
	{
		//MessageBoxA(NULL, "Create ShaderResourceView WIC Error", "Error Box", MB_OK);
	}

	hr = DirectX::GetMetadataFromDDSFile(fileName.c_str(), DirectX::DDS_FLAGS_NONE, mdata);
	hr = DirectX::LoadFromDDSFile(fileName.c_str(), DirectX::DDS_FLAGS_NONE, &mdata, *imageobj);
	hr = DirectX::CreateShaderResourceView(LGlobal::g_pDevice.Get(), imageobj->GetImages(), imageobj->GetImageCount(), mdata, &m_pTexSRV);

	if (SUCCEEDED(hr))
	{
		size.x = mdata.width;
		size.y = mdata.height;
		return true;
	}

	if (FAILED(hr))
	{
		//MessageBoxA(NULL, "Create ShaderResourceView DDS Error", "Error Box", MB_OK);
	}

	hr = DirectX::GetMetadataFromTGAFile(fileName.c_str(), DirectX::TGA_FLAGS_NONE, mdata);
	hr = DirectX::LoadFromTGAFile(fileName.c_str(), DirectX::TGA_FLAGS_NONE, &mdata, *imageobj);
	hr = DirectX::CreateShaderResourceView(LGlobal::g_pDevice.Get(), imageobj->GetImages(), imageobj->GetImageCount(), mdata, &m_pTexSRV);

	if (SUCCEEDED(hr))
	{
		size.x = mdata.width;
		size.y = mdata.height;
		return true;
	}

	if (FAILED(hr))
	{
		//MessageBoxA(NULL, "Create ShaderResourceView TGA Error", "Error Box", MB_OK);
	}

	return true;
}

const shared_ptr<::ScratchImage> LTexture::GetInfo()
{
	ComPtr<ID3D11Texture2D> texture;
	m_pTexSRV->GetResource((ID3D11Resource**)texture.ReleaseAndGetAddressOf());

	std::shared_ptr<DirectX::ScratchImage> image = std::make_shared<DirectX::ScratchImage>();
	HRESULT hr = DirectX::CaptureTexture(LGlobal::g_pDevice.Get(), LGlobal::g_pImmediateContext.Get(), texture.Get(), *image.get());

	if (FAILED(hr))
		MessageBoxA(NULL, "LTexture::GetInfo(), CaptureTexture Error", "Error Box", MB_OK);

	return image;
}

void LTexture::Save(wstring path)
{
	std::shared_ptr<DirectX::ScratchImage> srcimage = GetInfo();

	DirectX::SaveToWICFile(srcimage->GetImages(), srcimage->GetImageCount(),
		DirectX::WIC_FLAGS_NONE, DirectX::GetWICCodec(DirectX::WIC_CODEC_PNG),
		path.c_str());
}

void LTexture::CreateTexture(int width, int height)
{
	D3D11_TEXTURE2D_DESC desc;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&desc, sizeof(desc));
	ZeroMemory(&data, sizeof(data));

	BYTE* buf = new BYTE[width * height * 4];

	for (int i = 0; i < width * height * 4; i)
	{
		buf[i + 0] = 0;
		buf[i + 1] = 0;
		buf[i + 2] = 0;
		buf[i + 3] = 0;

		i += 4;
	}

	data.pSysMem = (void*)buf;
	data.SysMemPitch = width * 4;
	data.SysMemSlicePitch = 0;

	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	size.x = width;
	size.y = height;

	ComPtr<ID3D11Texture2D> texture;
	HRESULT hr = LGlobal::g_pDevice->CreateTexture2D(&desc, &data, texture.ReleaseAndGetAddressOf());

	if (FAILED(hr))
		//Utils::ShowErrorMessage(hr);

	hr = LGlobal::g_pDevice->CreateShaderResourceView(texture.Get(), nullptr, &m_pTexSRV);

	if (FAILED(hr))
		//Utils::ShowErrorMessage(hr);

	delete[] buf;
}

bool LTexture::Release()
{/*
	if (m_pTexSRV) m_pTexSRV->Release();
	m_pTexSRV = nullptr;*/
	return true;
}

LTexture::LTexture() {}
LTexture::~LTexture() {}

//Shader
void LShader::Apply()
{
	LGlobal::g_pImmediateContext->VSSetShader(m_pVS, NULL, 0);
	LGlobal::g_pImmediateContext->HSSetShader(NULL, NULL, 0);
	LGlobal::g_pImmediateContext->DSSetShader(NULL, NULL, 0);
	LGlobal::g_pImmediateContext->GSSetShader(NULL, NULL, 0);
	LGlobal::g_pImmediateContext->PSSetShader(m_pPS, NULL, 0);
}

bool LShader::Load(std::wstring fileName)
{
	LoadVertexShader(fileName);
	LoadPixelShader(fileName);

	return true;
}

bool LShader::LoadVertexShader(std::wstring fileName)
{
	HRESULT hr;
	ID3DBlob* ErrorCode;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
#ifdef _DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	hr = D3DCompileFromFile(
		fileName.c_str(),
		NULL,
		NULL,
		"VS",
		"vs_5_0",
		dwShaderFlags,
		NULL,
		&m_pVSBlob,
		&ErrorCode);



	if (FAILED(hr))
	{
		//ErrorCode
		TCHAR pMessage[500];
		mbstowcs(pMessage, (CHAR*)ErrorCode->GetBufferPointer(), 500);
		MessageBox(NULL, pMessage, L"ERROR", MB_OK);
		if (ErrorCode) ErrorCode->Release();
		return false;
	}

	hr = LGlobal::g_pDevice->CreateVertexShader(
		m_pVSBlob->GetBufferPointer(),
		m_pVSBlob->GetBufferSize(),
		NULL,
		&m_pVS);

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Create VertexShader Error", "Error Box", MB_OK);
		return false;
	}

	if (ErrorCode) ErrorCode->Release();

	return true;
}

bool LShader::LoadPixelShader(std::wstring fileName)
{
	ID3DBlob* ErrorCode;
	HRESULT hr;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
#ifdef _DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	hr = D3DCompileFromFile(
		fileName.c_str(),
		NULL,
		NULL,
		"PS",
		"ps_5_0",
		dwShaderFlags,
		NULL,
		&m_pPSBlob,
		&ErrorCode);

	if (FAILED(hr))
	{
		//ErrorCode
		TCHAR pMessage[500];
		mbstowcs(pMessage, (CHAR*)ErrorCode->GetBufferPointer(), 500);
		MessageBox(NULL, pMessage, L"ERROR", MB_OK);
		if (ErrorCode) ErrorCode->Release();
		return false;
	}

	hr = LGlobal::g_pDevice->CreatePixelShader(
		m_pPSBlob->GetBufferPointer(),
		m_pPSBlob->GetBufferSize(),
		NULL,
		&m_pPS);

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Create PixelShader Error", "Error Box", MB_OK);
		return false;
	}

	return true;
}

bool LShader::LoadPixelShader(std::wstring fileName, const char* funcName)
{
	ID3DBlob* ErrorCode;
	HRESULT hr;

	hr = D3DCompileFromFile(
		fileName.c_str(),
		NULL,
		NULL,
		funcName,
		"ps_5_0",
		NULL,
		NULL,
		&m_pPSBlob,
		&ErrorCode);

	if (FAILED(hr))
	{
		//ErrorCode
		TCHAR pMessage[500];
		mbstowcs(pMessage, (CHAR*)ErrorCode->GetBufferPointer(), 500);
		MessageBox(NULL, pMessage, L"ERROR", MB_OK);
		if (ErrorCode) ErrorCode->Release();
		return false;
	}

	hr = LGlobal::g_pDevice->CreatePixelShader(
		m_pPSBlob->GetBufferPointer(),
		m_pPSBlob->GetBufferSize(),
		NULL,
		&m_pPS);

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Create PixelShader Error", "Error Box", MB_OK);
		return false;
	}

	return true;
}

bool LShader::Release()
{
	if (m_pVSBlob) m_pVSBlob->Release();
	if (m_pPSBlob) m_pPSBlob->Release();
	if (m_pVS) m_pVS->Release();
	if (m_pPS) m_pPS->Release();
	m_pVSBlob = nullptr;
	m_pPSBlob = nullptr;
	m_pVS = nullptr;
	m_pPS = nullptr;

	return true;
}

LShader::LShader() {}
LShader::~LShader() {}