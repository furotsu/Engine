#pragma once 

#include "model.hpp"
#include "modelManager.hpp"

namespace engine
{
	class OpaqueInstances
	{
		std::shared_ptr<ShaderProgram> shader;
		uint32_t m_instanceCount;
	public:

		struct Instance
		{
			XMMATRIX pos;

			Instance() = delete;
			Instance(XMMATRIX mat) { pos = mat; }
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

		OpaqueInstances() = default;
		void init(std::shared_ptr<ShaderProgram> shader);

		uint32_t addModel(std::shared_ptr<Model> model, XMFLOAT3 size = { 10.0f, 10.0f, 10.0f });
		void addMaterial(Material& material, std::vector<Instance>& instances, uint32_t modelPos = 0u);

		void updateInstanceBuffers();
		void render();

		void clean();

		friend void loadModel(std::string path, std::shared_ptr<engine::Model> model);

	};

}