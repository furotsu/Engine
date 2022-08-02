#include "texture.hpp"
#include "textureLoader.hpp"
#include "debug.hpp"

void engine::Texture::init(std::string& filepath, TextureType type)
{
	ID3D11Resource* res;

	std::wstring widestr = std::wstring(filepath.begin(), filepath.end());
	const wchar_t* widecstr = widestr.c_str();

	HRESULT hr = DirectX::CreateDDSTextureFromFile(s_device, s_devcon, widecstr, &res, pRView.reset());
	ASSERT(hr >= 0 && L"unable to create texture from file");

	res->Release();
	res = nullptr;
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
