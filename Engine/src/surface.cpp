#include "surface.h"

Surface::Surface(math::Plane plane, const Material& material)
    : plane(plane), material(material)
{
}

bool Surface::hit(const math::ray& r, math::Intersection& hr)
{
    if (plane.hit(r, hr))
    {
        hr.material = material;
        return true;
    }
    return false;
}
