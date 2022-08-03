#include "shaderManager.hpp"

namespace engine
{
	ShaderManager::ShaderManager()
	{
	}

	void ShaderManager::init()
	{
		if (s_manager == nullptr)
		{
			s_manager = new ShaderManager();
		}
		else
		{
			ERROR("Initializing \" ShaderManager \" singleton more than once ");
		}
	}

	void ShaderManager::deinit()
	{
		if (s_manager == nullptr)
		{
			ERROR("Trying to delete \" ShaderManager \" singleton more than once ");
		}
		else
		{
			GetInstance()->clean();
			delete s_manager;
			s_manager = nullptr;
		}
	}

	ShaderManager* ShaderManager::GetInstance()
	{
#if defined( DEBUG ) || defined( _DEBUG )
		if (s_manager == nullptr)
		{
			ERROR("Trying to call \" ShaderManager \" singleton  instance before initializing it");
		}
#endif

		return s_manager;
	}

	std::shared_ptr<ShaderProgram> ShaderManager::getShader(std::vector<ShaderInfo> shaders, std::vector<D3D11_INPUT_ELEMENT_DESC> ied)
	{
		auto it1 = m_shaders.find(shaders[0].filePath);
		auto it2 = m_shaders.find(shaders[1].filePath);

		if ((it1 != m_shaders.end()) && (it2 != m_shaders.end()) && it1 == it2)
		{
			return it1->second;
		}
		else
		{
			ShaderProgram shader;
			shader.init(shaders, ied);
			std::shared_ptr<ShaderProgram> res = std::make_shared<ShaderProgram>(shader);
			m_shaders.emplace(std::make_pair(shaders[0].filePath, res));
			return res;
		}

		return nullptr;
	}

	std::shared_ptr<ShaderProgram> ShaderManager::getShader(std::vector<ShaderInfo> shaders)
	{
		auto it1 = m_shaders.find(shaders[0].filePath);
		auto it2 = m_shaders.find(shaders[1].filePath);

		if ((it1 != m_shaders.end()) && (it2 != m_shaders.end()) && it1 == it2)
		{
			return it1->second;
		}
		else
		{
			ShaderProgram shader;
			shader.init(shaders);
			std::shared_ptr<ShaderProgram> res = std::make_shared<ShaderProgram>(shader);
			m_shaders.emplace(std::make_pair(shaders[0].filePath, res));
			return res;
		}

		return nullptr;
	}


	void ShaderManager::clean()
	{
		for (auto& elem : GetInstance()->m_shaders)
		{
			elem.second->clean();
		}
	}
}