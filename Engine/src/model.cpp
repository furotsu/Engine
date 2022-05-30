#include <iostream>

#include "model.h"
#include "scene.h"

XMMATRIX Transform::toMat() const
{
	return XMMATRIX();
}

Scene::Sphere::Sphere(XMVECTOR position, float radius, const Material& material)
{
	this->center = position;
	this->radius = radius;
	this->material = material;
}

XMVECTOR Scene::Sphere::position()
{
	return this->center;
}

bool Scene::Sphere::hit(const math::ray& ray, ObjRef& outRef, math::Intersection& rec, Material*& outMaterial)
{
	if (math::Sphere::hit(ray, rec))
	{
		outRef.type = IntersectedType::Sphere;
		outRef.object = this;
		outMaterial = &material;
		return true;
	}
	return false;
}

XMVECTOR Scene::Model::position()
{
	return m_modelMat.r[3];
}

Scene::Model::Model(XMVECTOR position, XMVECTOR scale, const std::shared_ptr<Mesh>& mesh, const Material& material)
{
	m_mesh = mesh;
	m_transformation.position = position;
	m_transformation.scale = scale;
	this->material = material;

	m_modelMat = XMMatrixSet(
		XMVectorGetX(scale), 0.0f, 0.0f, 0.0f,
		0.0f, XMVectorGetY(scale), 0.0f, 0.0f,
		0.0f, 0.0f, XMVectorGetZ(scale), 0.0f,
		XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position), 1.0f
	);

	m_modelInvMat = XMMatrixInverse(nullptr, m_modelMat);
}

bool Scene::Model::hit(math::ray r, ObjRef& outRef, math::Intersection& rec, Material*& outMaterial)
{
	r.origin = XMVector4Transform(r.origin, m_modelInvMat);

	if (m_mesh->hit(r, rec))
	{
		outRef.type = IntersectedType::Model;
		outRef.object = this;
		outMaterial = &material;
		rec.point = XMVector4Transform(rec.point, m_modelMat);

		return true;
	}
	return false;
}

Scene::Surface::Surface(math::Plane plane, const Material& material)
	: plane(plane), material(material)
{
}

bool Scene::Surface::hit(const math::ray& r, ObjRef& outRef, math::Intersection& rec, Material*& outMaterial)
{
	if (plane.hit(r, rec))
	{
		outRef.type = IntersectedType::Surface;
		outRef.object = this;
		outMaterial = &material;
		return true;
	}
	return false;
}


