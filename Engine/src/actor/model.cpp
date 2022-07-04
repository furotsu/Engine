#include <iostream>

#include "scene.h"


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

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(Vertex) * 3;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = &m_mesh->m_vertices;

	s_device->CreateBuffer(&bd, &vertexData, m_pVBuffer.reset());       // create the buffer

	ASSERT(m_pVBuffer != 0 && "something wrong with vertex buffer");

	// copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	s_devcon->Map(m_pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, m_mesh->m_vertices.data(), sizeof(Vertex) * m_mesh->m_vertices.size());                 // copy the data
	s_devcon->Unmap(m_pVBuffer, NULL);                                     // unmap the buffer
}
