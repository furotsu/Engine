#pragma once

#include <memory>
#include <map>

#include "DxRes.hpp"
#include "texture.hpp"

namespace engine
{
	class TextureManager
	{
	protected:
		static TextureManager* s_manager;
		
		std::map<const wchar_t*, std::shared_ptr<Texture>> m_Textures;

	public:
		TextureManager();
		TextureManager(TextureManager& other) = delete;
		void operator=(const TextureManager&) = delete;

		static void init();
		static TextureManager* GetInstance();

		static std::shared_ptr<Texture> getTexture(const wchar_t* filepath, TextureType type = TextureType::TEXTURE_DEFAULT);
		static void clearUnusedTextures();


		static void clean();
	};
}