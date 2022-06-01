#include "mesh.h"


Mesh::Mesh(const std::vector<math::Triangle>& polygons)
    : m_polygons(polygons)
{
}


bool Mesh::hit(const math::ray& r, math::Intersection& rec, float tMin, float tMax) const
{
    bool foundIntersection = false;
    for (auto& triangle : m_polygons)
    {
        foundIntersection |= triangle.hit(r, rec, tMin, tMax);
    }

    return foundIntersection;
}
