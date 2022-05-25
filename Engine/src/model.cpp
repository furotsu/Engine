#include <iostream>

#include "model.h"

XMMATRIX Transform::toMat() const
{
	return XMMATRIX();
}

SphereModel::SphereModel(XMVECTOR position, float scale, Angles rotation, const std::shared_ptr<math::Sphere>& sphere, const Material& material)
{
	m_sphere = sphere;
	this->material = material;
	m_transformation.position = position;
	m_transformation.scale = XMVectorSet(scale, scale, scale, 0.0f);

	XMVECTOR rotQuat = XMQuaternionRotationRollPitchYaw(rotation.pitch, rotation.yaw, rotation.roll);
	m_modelMat = XMMatrixSet(
		scale, 0.0f, 0.0f, 0.0f,
		0.0f, scale, 0.0f, 0.0f,
		0.0f, 0.0f, scale, 0.0f,
		XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position), 1.0f
	);

	m_modelInvMat = XMMatrixInverse(nullptr, m_modelMat);
}

bool SphereModel::hit(math::ray r, math::Intersection& rec) const
{
	r.origin = XMVector4Transform(r.origin, m_modelInvMat);

	if (m_sphere->hit(r, rec))
	{
		rec.point = XMVector4Transform(rec.point, m_modelMat);
		rec.material = material;
		return true;
	}
	return false;
}

XMVECTOR SphereModel::position()
{
	return m_modelMat.r[3];
}

XMVECTOR Model::position()
{
	return m_modelMat.r[3];
}

Model::Model(XMVECTOR position, XMVECTOR scale, Angles rotation, const std::shared_ptr<Mesh>& mesh, const Material& material)
{
	m_mesh = mesh;
	m_transformation.position = position;
	m_transformation.scale = scale;
	m_material = material;

	XMVECTOR rotQuat = XMQuaternionRotationRollPitchYaw(rotation.pitch, rotation.yaw, rotation.roll);

	m_modelMat = XMMatrixSet(
		XMVectorGetX(scale), 0.0f, 0.0f, 0.0f,
		0.0f, XMVectorGetY(scale), 0.0f, 0.0f,
		0.0f, 0.0f, XMVectorGetZ(scale), 0.0f,
		XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position), 1.0f
	);

	m_modelInvMat = XMMatrixInverse(nullptr, m_modelMat);
}

bool Model::hit(math::ray r, math::Intersection& rec) const
{
	r.origin = XMVector4Transform(r.origin, m_modelInvMat);

	if (m_mesh->hit(r, rec))
	{
		rec.material = m_material;
		rec.point = XMVector4Transform(rec.point, m_modelMat);
		return true;
	}
	return false;
}
