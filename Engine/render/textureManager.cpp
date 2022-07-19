#include "textureManager.hpp"

namespace engine
{
	TextureManager::TextureManager()
	{
	}

	void TextureManager::init()
	{
		if (s_manager == nullptr)
		{
			s_manager = new TextureManager();
		}
		else
		{
			ERROR("Initializing \" TextureManager \" singleton more than once ");
		}
	}

	void TextureManager::deinit()
	{
		if (s_manager == nullptr)
		{
			ERROR("Trying to delete \" TextureManager \" singleton more than once ");
		}
		else
		{
			clean();
			delete s_manager;
			s_manager = nullptr;
		}
	}

	TextureManager* TextureManager::GetInstance()
	{
		if (s_manager == nullptr)
		{
			ERROR("Trying to call \" TextureManager \" singleton  instance before initializing it");
		}
		return s_manager;
	}

	std::shared_ptr<Texture> TextureManager::getTexture(const wchar_t* filepath, TextureType type)
	{
		auto it = GetInstance()->m_Textures.find(filepath);
		
		if (it != GetInstance()->m_Textures.end())
		{
			return it->second;
		}
		else
		{
			Texture tex;
			tex.init(filepath, type);
			std::shared_ptr<Texture> res = std::make_shared<Texture>(tex);
			GetInstance()->m_Textures.emplace(std::make_pair(filepath, res));
			return res;
		}
		return nullptr;
	}

	void TextureManager::clean()
	{
		for (auto& elem : GetInstance()->m_Textures)
		{
			elem.second->clean();
		}
	}
}