#include <iostream>

#include "scene.hpp"


void engine::Scene::Model::init(const std::shared_ptr<Mesh>& mesh)
{
	m_mesh = mesh;
	initBuffers();
}

void engine::Scene::Model::initBuffers()
{
	// create the vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_IMMUTABLE;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(Vertex) * 3;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = NULL;    // allow CPU to write in buffer

	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = &m_mesh->m_vertices[0];

	s_device->CreateBuffer(&bd, &vertexData, m_pVBuffer.reset());       // create the buffer

	ASSERT(m_pVBuffer != 0 && "something wrong with vertex buffer");


}

void engine::Scene::Model::cleanBuffers()
{
	m_pVBuffer.release();
}
