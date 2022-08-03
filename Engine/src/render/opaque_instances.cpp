#include "opaque_instances.hpp"
#include "assimpLoader.hpp"

namespace engine
{
	void OpaqueInstances::init(std::shared_ptr<ShaderProgram> shader)
	{
		this->shader = shader;
		shader->createUniform(sizeof(XMMATRIX));
	}

	uint32_t OpaqueInstances::addModel(std::shared_ptr<Model> model, XMFLOAT3 size)
	{
		perModel.push_back({ model });

		uint32_t perModelPos = perModel.size() - 1;

		for (uint32_t i = 0; i != perModel[perModelPos].model->meshes.size(); ++i)
		{
			perModel[perModelPos].perMesh.push_back(PerMesh());

			XMMATRIX& transformDst = perModel[perModelPos].model->meshes[i].m_transform;
			transformDst = XMMatrixMultiply(transformDst, XMMatrixScaling(size.x, size.y, size.z));

		}

		return perModelPos;
	}

	void OpaqueInstances::addMaterial(Material& material, std::vector<Instance>& instances, uint32_t modelPos)
	{
		for (auto& mesh : perModel[modelPos].perMesh)
		{
			mesh.perMaterial.push_back(PerMaterial());
		}

		for (auto& mesh : perModel[modelPos].perMesh)
		{
			for (uint32_t i = 0; i != instances.size(); ++i)
				mesh.perMaterial[mesh.perMaterial.size() - 1].instances.push_back(instances[i]);
		}

		for (auto& mesh : perModel[modelPos].model->meshes)
		{
			if(material.texture)
				mesh.materials.push_back(material);
		}

		updateInstanceBuffers();
	}

	void OpaqueInstances::updateInstanceBuffers()
	{
		uint32_t totalInstances = 0;
		for (auto& model : perModel)
			for (auto& perMesh : model.perMesh)
				for (const auto& material : perMesh.perMaterial)
					totalInstances += uint32_t(material.instances.size());

		if (totalInstances == 0)
			return;

		instanceBuffer.init(totalInstances * sizeof(Instance), D3D11_USAGE_DYNAMIC); // resizes if needed

		auto mapping = instanceBuffer.map();
		Instance* dst = static_cast<Instance*>(mapping.pData);

		uint32_t copiedNum = 0;
		for (const auto& model : perModel)
		{
			for (uint32_t meshIndex = 0; meshIndex < model.perMesh.size(); ++meshIndex)
			{
				const Mesh& mesh = model.model->model().meshes[meshIndex];

				for (const auto& material : model.perMesh[meshIndex].perMaterial)
				{
					auto& instances = material.instances;

					uint32_t numModelInstances = instances.size();
					for (uint32_t index = 0; index < numModelInstances; ++index)
					{
						dst[copiedNum++] = material.instances[index];
					}
				}
			}
		}

		instanceBuffer.unmap();
	}

	void OpaqueInstances::render()
	{

		if (instanceBuffer.size() == 0)
			return;

		shader->bind();
		instanceBuffer.bind(1, sizeof(Instance));

		uint32_t renderedInstances = 0;
		s_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		for (const auto& model : perModel)
		{
			if (model.empty()) continue;

			model.model->vertexBuffer().bind(0, sizeof(Mesh::Vertex));
			model.model->indexBuffer().bind();

			for (uint32_t meshIndex = 0; meshIndex < model.perMesh.size(); ++meshIndex)
			{
				const Mesh& mesh = model.model->model().meshes[meshIndex];
				const auto& meshRange = model.model->meshRange(meshIndex);

				std::vector<const void*> data;
				data.push_back(&mesh.m_transform);
				shader->bindUniforms(data);

				for (uint32_t materialIndex = 0; materialIndex < model.perMesh[meshIndex].perMaterial.size(); ++materialIndex)
				{
					const auto& perMaterial = model.perMesh[meshIndex].perMaterial[materialIndex];
					if (perMaterial.instances.empty()) continue;

					const auto& material = mesh.materials[materialIndex];
					material.texture->bind();

					uint32_t numInstances = uint32_t(perMaterial.instances.size());
					s_devcon->DrawIndexedInstanced(meshRange.indexNum, numInstances, meshRange.indexOffset, meshRange.vertexOffset, renderedInstances);
					renderedInstances += numInstances;
				}
			}
		}
	}
	void OpaqueInstances::clean()
	{
		shader = nullptr;
		for (auto& model : perModel)
			model.model->clean();


		instanceBuffer.clean();
	}
}