#include "scene.h"

Scene::SphereMover::SphereMover(Scene::Sphere* sphere, const XMVECTOR& pickedPos)
	: sphere(sphere), pickedPosition(pickedPos)
{
}

Scene::SphereMover::SphereMover(Scene::Sphere* sphere)
	: sphere(sphere), pickedPosition(sphere->position())
{
}

void Scene::SphereMover::moveTo(const XMVECTOR& offset)
{
	sphere->center = offset;
	pickedPosition = offset;
}

void Scene::SphereMover::moveBy(const XMVECTOR& offset)
{
	sphere->center += offset;
	pickedPosition += offset;
}

XMVECTOR Scene::SphereMover::getPickedPos()
{
	return pickedPosition;
}

Scene::ModelMover::ModelMover(Model* model, const XMVECTOR& pickedPosition)
	: model(model), pickedPosition(pickedPosition)
{
}

Scene::ModelMover::ModelMover(Model* model)
	: model(model), pickedPosition(model->position())
{
}

void Scene::ModelMover::moveTo(const XMVECTOR& position)
{
	model->m_modelMat.r[3] = position;
	model->m_modelInvMat = XMMatrixInverse(nullptr, model->m_modelMat);
	pickedPosition = position;
}

void Scene::ModelMover::moveBy(const XMVECTOR& position)
{
	model->m_modelMat.r[3] += position;
	model->m_modelInvMat = XMMatrixInverse(nullptr, model->m_modelMat);
	pickedPosition += position;
}

XMVECTOR Scene::ModelMover::getPickedPos()
{
	return pickedPosition;
}

Scene::PointLightMover::PointLightMover(PointLight* pointLight, const XMVECTOR& pickedPosition)
	: pointLight(pointLight), pickedPosition(pickedPosition)
{
}

Scene::PointLightMover::PointLightMover(PointLight* pointLight)
	: pointLight(pointLight), pickedPosition(pointLight->center)
{
}

void Scene::PointLightMover::moveTo(const XMVECTOR& position)
{
	pointLight->center = position;
	pickedPosition = position;
}

void Scene::PointLightMover::moveBy(const XMVECTOR& offset)
{
	pointLight->center += offset;
	pickedPosition += offset;
}

XMVECTOR Scene::PointLightMover::getPickedPos()
{
	return pickedPosition;
}

Scene::FlashLightMover::FlashLightMover(SpotLight* pointLight, const XMVECTOR& pickedPosition)
	: flashLight(pointLight), pickedPosition(pickedPosition)
{
}

Scene::FlashLightMover::FlashLightMover(SpotLight* pointLight)
	: flashLight(pointLight), pickedPosition(pointLight->center)
{
}

void Scene::FlashLightMover::moveTo(const XMVECTOR& position)
{
	flashLight->center = position;
	pickedPosition = position;
}

void Scene::FlashLightMover::moveBy(const XMVECTOR& offset)
{
	flashLight->center += offset;
	pickedPosition += offset;
}

XMVECTOR Scene::FlashLightMover::getPickedPos()
{
	return pickedPosition;
}
