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
			ERROR("Initializing \" Globals \" singleton more than once ");
		}
	}

	TextureManager* TextureManager::GetInstance()
	{
		if (s_manager == nullptr)
		{
			ERROR("Trying to call \" Globals \" singleton  instance before initializing it");
		}
		return s_manager;
	}

	std::shared_ptr<Texture> TextureManager::getTexture(const wchar_t* filepath, TextureType type)
	{
		std::map<const wchar_t*, std::shared_ptr<Texture>>::iterator it = GetInstance()->m_Textures.find(filepath);
		
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

	void TextureManager::clearUnusedTextures()
	{
		//std::map<const wchar_t*, std::shared_ptr<Texture>>::iterator iter = GetInstance()->m_Textures.begin();
		//std::map<const wchar_t*, std::shared_ptr<Texture>>::iterator endIter = GetInstance()->m_Textures.end();
		//
		//for (; iter != endIter; )
		//{
		//	if (iter->second.unique())
		//	{
		//		iter = GetInstance()->m_Textures.erase(iter);
		//	}
		//	else
		//	{
		//		iter++;
		//	}
		//}
	}

	void TextureManager::clean()
	{
		for (auto& elem : GetInstance()->m_Textures)
		{
			elem.second->clean();
		}
	}
}