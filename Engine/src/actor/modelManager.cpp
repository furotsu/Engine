#include "modelManager.hpp"
#include "assimpLoader.hpp"

namespace engine
{
	ModelManager::ModelManager()
	{
	}

	void ModelManager::init()
	{
		if (s_manager == nullptr)
		{
			s_manager = new ModelManager();
		}
		else
		{
			ERROR("Initializing \" ModelManager \" singleton more than once ");
		}
	}

	void ModelManager::deinit()
	{
		if (s_manager == nullptr)
		{
			ERROR("Trying to delete \" ModelManager \" singleton more than once ");
		}
		else
		{
			clean();
			delete s_manager;
			s_manager = nullptr;
		}
	}

	ModelManager* ModelManager::GetInstance()
	{
		if (s_manager == nullptr)
		{
			ERROR("Trying to call \" TextureManager \" singleton  instance before initializing it");
		}
		return s_manager;
	}

	std::shared_ptr<Model> ModelManager::getModel(std::string filepath, std::string filename)
	{
		auto it = GetInstance()->m_Models.find(filepath);

		if (it != GetInstance()->m_Models.end())
		{
			return it->second;
		}
		else
		{
			std::shared_ptr<Model> model = std::make_shared<Model>();
			assimp::loadModel(filepath, filename, model);


			GetInstance()->m_Models.emplace(std::make_pair(filepath, model));
			return model;
		}
		return nullptr;
	}

	void ModelManager::clean()
	{
		for (auto& elem : GetInstance()->m_Models)
		{
			elem.second->clean();
		}
	}
}