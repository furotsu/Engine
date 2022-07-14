#include "texture.hpp"
#include "textureLoader.hpp"
#include "debug.hpp"

void engine::Texture::init(const wchar_t* filepath, TextureType type)
{
	ID3D11Resource* res;
	HRESULT hr = DirectX::CreateDDSTextureFromFile(s_device, s_devcon, filepath, &res, pRView.reset());
	res->Release();
	res = nullptr;

	ASSERT(hr >= 0 && L"unable to create texture from file");

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = s_device->CreateSamplerState(&sampDesc, pTSampleState.reset());
}

void engine::Texture::clean()
{
	pRView.release();
	pTSampleState.release();
}

void engine::Texture::bind()
{
	s_devcon->PSSetShaderResources(0, 1, pRView.access());
	s_devcon->PSSetSamplers(0, 1, pTSampleState.access());
}

void engine::Texture::unbind()
{
	s_devcon->PSSetShaderResources(0, 1, NULL);
	s_devcon->PSSetSamplers(0, 1, NULL);
}
