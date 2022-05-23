#pragma once

#include <memory>

#include "mesh.h"
#include "sphere.h"
#include "quaternion.h"
#include "material.h"



class IobjectMover;
class SphereMover;
class MatrixMover;
class ModelMover;


struct Angles
{
	float roll;
	float pitch;
	float yaw;

	Angles() = default;
	Angles(float r, float p, float y)
		: roll(r), pitch(p), yaw(y)
	{}
};

struct Transform // We can build a transformation matrix from this
{
	math::Quaternion rotation;
	XMVECTOR position;
	XMVECTOR scale;

	XMMATRIX toMat() const;

	Transform() = default;
};

class SphereModel
{
public:
	Transform m_transformation;
	XMMATRIX  m_modelMat;
	XMMATRIX  m_modelInvMat;
	std::shared_ptr<math::Sphere> m_sphere;

	Material material;

	SphereModel() = default;
	SphereModel(XMVECTOR position, float scale, Angles rotation, const std::shared_ptr<math::Sphere>& sphere, const Material& material = Material());

	bool hit(math::ray r, math::Intersection& rec) const;

	XMVECTOR position();

	friend class SphereMover;
};

class Model
{
public:
	Transform m_transformation;
	XMMATRIX m_modelMat;
	XMMATRIX m_modelInvMat;
	std::shared_ptr<Mesh> m_mesh;
	Material m_material;


	Model() = default;
	Model(XMVECTOR position, XMVECTOR scale, Angles rotation, const std::shared_ptr<Mesh>& mesh, const Material& material = Material());

	XMVECTOR position();

	bool hit(math::ray r, math::Intersection& rec) const;

	friend class modelMover;
};