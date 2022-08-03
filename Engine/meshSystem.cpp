#include "modelManager.hpp"
#include "shaderManager.hpp"
#include "meshSystem.hpp"


namespace engine
{
	MeshSystem::MeshSystem()
	{
	}

	void MeshSystem::init()
	{
		if (s_manager == nullptr)
		{
			s_manager = new MeshSystem();
		}
		else
		{
			ERROR("Initializing \" ModelManager \" singleton more than once ");
		}
	}

	void MeshSystem::deinit()
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

	MeshSystem* MeshSystem::GetInstance()
	{
		if (s_manager == nullptr)
		{
			ERROR("Trying to call \" TextureManager \" singleton  instance before initializing it");
		}
		return s_manager;
	}

	void MeshSystem::initOpaqueInstances(std::vector<ShaderInfo>& shaders, std::vector<D3D11_INPUT_ELEMENT_DESC>& ied)
	{
		OpaqueInstances i = OpaqueInstances();
		i.init(ShaderManager::GetInstance()->getShader(shaders, ied));
		m_opaqueInstances.push_back(i);
	}

	void MeshSystem::addOpaqueInstances(std::shared_ptr<Model> model, std::vector<std::vector<OpaqueInstances::Instance>> instances, std::vector<Material> materials)
	{
		uint32_t modelPos = m_opaqueInstances[0].addModel(model);

		for (uint32_t i = 0; i != materials.size(); ++i)
		{
			m_opaqueInstances[0].addMaterial(materials[i], instances[i], modelPos);
		}
	}
		
	void MeshSystem::render()
	{
		for (auto& OI : m_opaqueInstances)
			OI.render();
	}

	void MeshSystem::clean()
	{
		for (auto& elem : GetInstance()->m_opaqueInstances)
		{
			elem.clean();
		}
	}
}