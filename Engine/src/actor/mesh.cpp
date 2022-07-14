#include "mesh.hpp"


engine::Mesh::Mesh(const std::vector<Vertex>& vertices)
{
    m_vertices = vertices;
}

engine::Mesh::Mesh(const std::vector<Vertex>& vertices, std::vector<uint32_t> indices)
{
    m_vertices = vertices;
    m_indices = indices;
}

bool engine::Mesh::hit(const math::ray& r, math::Intersection& rec, float tMin, float tMax) const
{
    return false;
}
