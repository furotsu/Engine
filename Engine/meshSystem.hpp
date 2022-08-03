#pragma once

#include <memory>
#include <map>

#include "DxRes.hpp"
#include "opaque_instances.hpp"

namespace engine
{
	class MeshSystem
	{
	protected:
		static MeshSystem* s_manager;

		std::vector<OpaqueInstances> m_opaqueInstances;

	public:
		MeshSystem();
		MeshSystem(MeshSystem& other) = delete;
		void operator=(const MeshSystem&) = delete;

		static void init();
		static void deinit();
		static MeshSystem* GetInstance();

		void initOpaqueInstances(std::vector<ShaderInfo>& shaders, std::vector<D3D11_INPUT_ELEMENT_DESC>& ied);
		void addOpaqueInstances(std::shared_ptr<Model> model, std::vector<std::vector<OpaqueInstances::Instance>> instances, std::vector<Material> materials = { {} });
		void render();

		static void clean();
	};
}