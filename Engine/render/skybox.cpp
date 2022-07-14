#include "skybox.hpp"

engine::Skybox::Skybox(const wchar_t* filepath)
{
	m_texture = TextureManager::getTexture(filepath, TextureType::TEXTURE_CUBEMAP);
}

void engine::Skybox::bind()
{
	m_texture->bind();
}

void engine::Skybox::clean()
{
	m_texture = nullptr;
}
