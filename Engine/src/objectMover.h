#pragma once

#include "sphere.h"
#include "model.h"


class IobjectMover
{
public:
	virtual void moveTo(const XMVECTOR&) = 0;
	virtual void moveBy(const XMVECTOR&) = 0;
	virtual XMVECTOR getPickedPos()      = 0;

};

class SphereMover : public IobjectMover
{
public:
	SphereModel& sphere;
	XMVECTOR pickedPosition;

	SphereMover() = default;
	SphereMover(SphereModel&, const XMVECTOR&);
	SphereMover(const SphereMover&);


	virtual void moveTo(const XMVECTOR&) override;
	virtual void moveBy(const XMVECTOR&) override;
	virtual XMVECTOR getPickedPos()  override;
};

class ModelMover : public IobjectMover
{
public:
	Model& model;
	XMVECTOR pickedPosition;

	ModelMover() = default;
	ModelMover(Model&, const XMVECTOR&);
	ModelMover(const ModelMover&);


	virtual void moveTo(const XMVECTOR&) override;
	virtual void moveBy(const XMVECTOR&) override;
	virtual XMVECTOR getPickedPos() override;

};

class MatrixMover : public IobjectMover
{
public:
	XMMATRIX matrix;

	MatrixMover(XMMATRIX&);
	virtual void moveTo(const XMVECTOR&) override;
};

class TransformMover : public IobjectMover
{
public:
	Transform& transform;

	TransformMover(Transform&);
	virtual void moveTo(const XMVECTOR&) override;
};