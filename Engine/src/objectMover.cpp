#include "objectMover.h"

SphereMover::SphereMover(SphereModel& sphere, const XMVECTOR& pickedPos)
	:	sphere(sphere), pickedPosition(pickedPos)
{
}

SphereMover::SphereMover(const SphereMover& sm2)
	: sphere(sm2.sphere), pickedPosition(sm2.pickedPosition)
{
}


void SphereMover::moveTo(const XMVECTOR& offset)
{
	sphere.m_modelMat.r[3] = offset;
	sphere.m_modelInvMat = XMMatrixInverse(nullptr, sphere.m_modelMat);	
	pickedPosition = offset;
}

void SphereMover::moveBy(const XMVECTOR& offset)
{
	sphere.m_modelMat.r[3] += offset;
	sphere.m_modelInvMat = XMMatrixInverse(nullptr, sphere.m_modelMat);
	pickedPosition += offset;
}

XMVECTOR SphereMover::getPickedPos()
{
	return pickedPosition;
}

ModelMover::ModelMover(Model& model, const XMVECTOR& pickedPos)
	: model(model), pickedPosition(pickedPos)
{
}

ModelMover::ModelMover(const ModelMover& modelMov)
	:model(modelMov.model), pickedPosition(modelMov.pickedPosition)
{

}

void ModelMover::moveTo(const XMVECTOR& position)
{
	model.m_modelMat.r[3] = position;
	model.m_modelInvMat = XMMatrixInverse(nullptr, model.m_modelMat);
	pickedPosition = position;
}

void ModelMover::moveBy(const XMVECTOR& position)
{
	model.m_modelMat.r[3] += position;
	model.m_modelInvMat = XMMatrixInverse(nullptr, model.m_modelMat);
	pickedPosition += position;
}

XMVECTOR ModelMover::getPickedPos()
{
	return pickedPosition;
}
