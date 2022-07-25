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

}

void engine::Texture::clean()
{
	pRView.release();
}

void engine::Texture::bind()
{
	s_devcon->PSSetShaderResources(0, 1, pRView.access());
}

void engine::Texture::unbind()
{
	s_devcon->PSSetShaderResources(0, 1, NULL);
	s_devcon->PSSetSamplers(0, 1, NULL);
}
