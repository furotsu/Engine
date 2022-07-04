#include "mesh.h"


engine::Mesh::Mesh(const std::vector<Vertex>& vertices)
{
    m_vertices = vertices;
}

bool engine::Mesh::hit(const math::ray& r, math::Intersection& rec, float tMin, float tMax) const
{
    return false;
}
