#pragma once 

#include "model.hpp"
#include "modelManager.hpp"

namespace engine
{
	class OpaqueInstances
	{
	protected:
		std::shared_ptr<ShaderProgram> shader;
		uint32_t m_instanceCount;

		struct Instance
		{
			XMFLOAT3 pos;
		};

		struct PerMaterial
		{
			std::vector<Instance> instances;
		};

		struct PerMesh
		{
			std::vector<PerMaterial> perMaterial;
		};

		struct PerModel
		{
			std::shared_ptr<Model> model;
			std::vector<PerMesh> perMesh;

			bool empty() const { return model == nullptr; }

		};

		VertexBuffer<Instance> instanceBuffer;
		std::vector<PerModel> perModel;
	public:

		OpaqueInstances() = default;
		void init(std::shared_ptr<ShaderProgram> shader);

		void addModel(std::string& filepath, std::string& filename, std::vector<XMFLOAT3>& positions, XMFLOAT3 size = {10.0f, 10.0f, 10.0f});


		void updateInstanceBuffers();
		void render();

		void clean();

		friend void loadModel(std::string path, std::shared_ptr<engine::Model> model);
	};

	class MeshSystem
	{
		OpaqueInstances opaqueInstances;

		void render()
		{
			opaqueInstances.render();
		}
	};
}