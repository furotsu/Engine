#include <iostream>

#include "model.hpp"

namespace engine
{
	void IndexBuffer::init(std::vector<Mesh::Triangle>& indices, D3D11_USAGE usage)
	{
		D3D11_BUFFER_DESC indexBufferDesc;
		ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

		indexBufferDesc.Usage = usage;
		indexBufferDesc.ByteWidth = sizeof(uint32_t) * indices.size() * 3; // every triangle has 3 indices
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = (usage == D3D11_USAGE_DYNAMIC) ? D3D11_CPU_ACCESS_WRITE : NULL;
		indexBufferDesc.MiscFlags = NULL;

		D3D11_SUBRESOURCE_DATA indexData;
		ZeroMemory(&indexData, sizeof(indexData));
		indexData.pSysMem = &indices[0];

		s_device->CreateBuffer(&indexBufferDesc, &indexData, m_pIBuffer.reset());
	};

	void Model::init(std::vector<Mesh::Vertex>& vertices, std::vector<Mesh::Triangle> indices)
	{
		m_vertices.init(vertices, D3D11_USAGE_IMMUTABLE);
		m_indices.init(indices, D3D11_USAGE_IMMUTABLE);
	}

	void Model::clean()
	{
		for (auto& mesh : meshes)
			mesh.clean();
		m_indices.clean();
		m_vertices.clean();
	}
}