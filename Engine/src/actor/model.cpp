#include <iostream>

#include "model.hpp"

engine::Model::Model(const std::shared_ptr<Mesh>& mesh, const wchar_t* textureFilepath, float scale, XMVECTOR position)
{
	m_transformation.scale = XMVectorSet(scale, scale, scale, 0.0f);
	m_transformation.position = position;
	m_transformation.rotation = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_mesh = mesh;
	m_texture = TextureManager::getTexture(textureFilepath);
	initBuffers();
}

void engine::Model::initBuffers()
{
	// create the vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_IMMUTABLE;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(Vertex) * m_mesh->m_vertices.size();             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = NULL;    // don't allow CPU to write in buffer

	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = &m_mesh->m_vertices[0];

	s_device->CreateBuffer(&bd, &vertexData, m_pVBuffer.reset()); // create the buffer

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = sizeof(uint32_t) * m_mesh->m_indices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = NULL;
	indexBufferDesc.MiscFlags = NULL;

	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory(&indexData, sizeof(indexData));

	indexData.pSysMem = &m_mesh->m_indices[0];

	s_device->CreateBuffer(&indexBufferDesc, &indexData, m_pIBuffer.reset());

	s_devcon->IASetIndexBuffer(m_pIBuffer, DXGI_FORMAT_R32_UINT, 0);


	ASSERT(m_pVBuffer != 0 && "something wrong with vertex buffer");

}

void engine::Model::cleanBuffers()
{
	m_pVBuffer.release();
	m_pIBuffer.release();
	m_texture = nullptr;
}

void engine::Model::bindTexture()
{
	m_texture->bind();
}

XMMATRIX engine::Model::getModelMat()
{
	return m_transformation.toMat();
}
