#include "scene.h"
#include <math.h>
#include <algorithm>

// For each pixel determine what color it should 
//	   based on whether ray hit any object or not 
XMVECTOR Scene::getPixelColor(const ray& r,  const XMVECTOR& cameraPos)
{
	Material* material;
	Intersection hr;
	hr.reset();

	XMVECTOR resColor = XMVectorSet(0.678f, 0.847f, 0.902f, 0.0f);

	if (findIntersection(r, hr, material))
	{
		resColor = material->emmision + m_ambientLight * material->albedo;

		for (auto& l0 : m_pointLights)
			resColor += illuminate(hr, material, cameraPos, l0);

		for (auto& l1 : m_directionalLights)
			resColor += illuminate(hr, material, cameraPos, l1);

		for (auto& l2 : m_flashLights)
			resColor += illuminate(hr, material, cameraPos, l2);
	}
	
	return resColor;
}

bool Scene::findIntersection(const math::ray& r, Intersection& outNearest,  Material*& outMaterial)
{
	ObjRef ref = { nullptr, IntersectedType::NUM };

	findIntersectionInternal(r, ref, outNearest, outMaterial);

	return ref.type != IntersectedType::NUM;
}


bool Scene::findIntersection(const ray& r, IntersectionQuery& query)
{
	ObjRef ref = { nullptr, IntersectedType::NUM };

	findIntersectionInternal(r, ref, query.intersection, query.materialPtr);
	switch (ref.type)
	{
	case IntersectedType::Sphere:
	{
		Scene::Sphere* sphere = static_cast<Scene::Sphere*>(ref.object);
		query.mover.reset(new SphereMover(sphere, query.intersection.point));
		distToPickedObj = query.intersection.hitParam;
	}break;
	case IntersectedType::Model:
	{
		Model* model = static_cast<Model*>(ref.object);
		query.mover.reset(new ModelMover(model, query.intersection.point));
		distToPickedObj = query.intersection.hitParam;
	}break;
	case IntersectedType::PointLight:
	{
		PointLight* model = static_cast<PointLight*>(ref.object);
		query.mover.reset(new PointLightMover(model, query.intersection.point));
		distToPickedObj = query.intersection.hitParam;
	}break;
	case IntersectedType::FlashLight:
	{
		FlashLight* model = static_cast<FlashLight*>(ref.object);
		query.mover.reset(new FlashLightMover(model, query.intersection.point));
		distToPickedObj = query.intersection.hitParam;
	}break;
	}
	return ref.type != IntersectedType::NUM;
}

void Scene::findIntersectionInternal(const ray& r, ObjRef& outRef, Intersection& outNearest, Material*& outMaterial)
{
	bool foundIntersection = false;

	foundIntersection |= m_surface.hit(r, outRef, outNearest, outMaterial);

	for (auto& sphere : m_spheres)
		foundIntersection |= sphere.hit(r, outRef, outNearest, outMaterial);

	for (auto& model : m_models)
		foundIntersection |= model.hit(r, outRef, outNearest, outMaterial);

	for (auto& light : m_pointLights)
		foundIntersection |= light.hit(r, outRef, outNearest, outMaterial);

	for (auto& light : m_flashLights)
		foundIntersection |= light.hit(r, outRef, outNearest, outMaterial);
}

XMVECTOR Scene::illuminate(const Intersection& hr, Material*& material, const XMVECTOR& cameraPos, const PointLight& light)
{
	// adding small offset to avoid self-shadowing artifact
	ray r(hr.point + 1.0f * hr.normal, XMVector3Normalize(light.getCenter() - hr.point));

	Intersection tmp;
	tmp.reset();
	tmp.hitParam = XMVectorGetX(XMVector3Length(hr.point - light.getCenter())) - light.getRadius() - 1.0f;

	Material* materialShadow;

	if (findIntersection(r, tmp, materialShadow))
	{
		return XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	}
	else
	{
		return light.illuminate(hr.point, hr.normal, cameraPos, material);
	}
}

XMVECTOR Scene::illuminate(const Intersection& hr, Material*& material, const XMVECTOR& cameraPos, const DirectionalLight& light)
{
	// adding small offset to avoid self-shadowing artifact
	ray r(hr.point + 0.01f * hr.normal, -light.m_direction);

	Intersection tmp;
	tmp.reset();
	Material* materialShadow;


	if (findIntersection(r, tmp, materialShadow))
	{
		return XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	}
	else
	{
		return light.illuminate(hr.point, hr.normal, cameraPos, material);
	}
}

XMVECTOR Scene::illuminate(const Intersection& hr, Material*& material, const XMVECTOR& cameraPos, const FlashLight& light)
{
	// adding small offset to avoid self-shadowing artifact
	ray r(hr.point + 1.0f * hr.normal, XMVector3Normalize(light.getCenter() - hr.point));

	Intersection tmp;
	tmp.reset();
	tmp.hitParam = XMVectorGetX(XMVector3Length(hr.point - light.getCenter())) - light.getRadius() - 1.0f;
	Material* materialShadow;

	if (findIntersection(r, tmp, materialShadow))
	{
		return  XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	}
	else
	{
		return light.illuminate(hr.point, hr.normal, cameraPos, material);
	}
}

void Scene::render(Window& window, Camera& camera)
{
	ray r;
	XMVECTOR point;

	r.origin = camera.position();
	
	XMVECTOR TL = XMVectorSet(-1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR TR = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR BR = XMVectorSet(1.0f, -1.0f, 1.0f, 1.0f);
	XMVECTOR BL = XMVectorSet(-1.0f, -1.0f, 1.0f, 1.0f);

	TL = XMVector4Transform(TL, camera.m_projInv);
	TR = XMVector4Transform(TR, camera.m_projInv);
	BR = XMVector4Transform(BR, camera.m_projInv);
	BL = XMVector4Transform(BL, camera.m_projInv);

	TL /=  XMVectorGetW(TL);
	TR /=  XMVectorGetW(TR);
	BR /=  XMVectorGetW(BR);
	BL /=  XMVectorGetW(BL);

	TL = XMVector4Transform(TL, camera.m_viewInv);
	TR = XMVector4Transform(TR, camera.m_viewInv);
	BR = XMVector4Transform(BR, camera.m_viewInv);
	BL = XMVector4Transform(BL, camera.m_viewInv);

	for (int h = 0; h < window.canvas.getHeight(); h++)
	{
		for (int w = 0; w < window.canvas.getWidth(); w++)
		{
			r.direction =  -r.origin + (BL + (BR - BL) * (w + 0.5f)/window.canvas.getWidth() + (TL - BL) * (window.canvas.getHeight() - h + 0.5f) / window.canvas.getHeight());
			r.direction = XMVector3Normalize(XMVectorSet(XMVectorGetX(r.direction), XMVectorGetY(r.direction), XMVectorGetZ(r.direction), 0.0f));
			XMVECTOR col = getPixelColor(r, camera.position());

			float r = XMVectorGetX(col) < 1.0f ? XMVectorGetX(col) : 0.99f;
			float g = XMVectorGetY(col) < 1.0f ? XMVectorGetY(col) : 0.99f;
			float b = XMVectorGetZ(col) < 1.0f ? XMVectorGetZ(col) : 0.99f;
			
			window.canvas.setPixel(w, h, r * 255, g * 255, b * 255);
		}
	}
}

void Scene::addSphere(const Sphere& sphereModel)
{
	m_spheres.push_back(sphereModel);
}

void Scene::addModel(Model model)
{
	m_models.push_back(model);
}

void Scene::setSurface(Surface surface)
{
	m_surface = surface;
}

void Scene::setAmbient(XMVECTOR ambient) { m_ambientLight = ambient; }

void Scene::addPointLight(const PointLight& light) {m_pointLights.push_back(light);}
void Scene::addDirLight(const DirectionalLight& light) { m_directionalLights.push_back(light); }
void Scene::addFlashLight(const FlashLight& light) { m_flashLights.push_back(light); }

void Scene::pickObject(const Camera& camera, const XMVECTOR& mousePos)
{
	XMVECTOR point = XMVector4Transform(mousePos, camera.m_projInv);
	point /= XMVectorGetW(point);
	point = XMVector4Transform(point, camera.m_viewInv);

	ray r(camera.position(), point);
	r.direction = -r.origin + point;
	r.direction = XMVector3Normalize(XMVectorSet(XMVectorGetX(r.direction), XMVectorGetY(r.direction), XMVectorGetZ(r.direction), 0.0f));

	pickedObjMoverQuery.intersection.reset();
	findIntersection(r, pickedObjMoverQuery);
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

XMVECTOR Scene::PointLight::illuminate(const XMVECTOR& fragPos, const XMVECTOR& fragNorm, const XMVECTOR& cameraPos, Material*& material) const
{
	XMVECTOR viewDirection = XMVector3Normalize(cameraPos - fragPos);
	XMVECTOR lightDirection = XMVector3Normalize(this->center - fragPos);
	float distance = XMVectorGetX(XMVector3Length(this->center - fragPos));

	XMVECTOR halfDir = XMVector3Normalize(lightDirection + viewDirection);

	//diff
	float attenuation = 1.0 / (m_quadraticIntens * distance * distance);

	XMVECTOR diffuse = max(XMVectorGetX(XMVector3Dot(fragNorm, lightDirection)), 0.0f) * material->albedo * attenuation;

	//spec
	float spec = pow(max(XMVectorGetX(XMVector3Dot(fragNorm, halfDir)), 0.0f), material->glossines) * material->specular;
	XMVECTOR specular = XMVectorSet(spec, spec, spec, 0.0f);


	XMVECTOR Li = m_lightColor / (m_linearIntens * distance);
	return  Li * (diffuse + specular);
}

XMVECTOR Scene::DirectionalLight::illuminate(const XMVECTOR& fragPos, const XMVECTOR& fragNorm, const XMVECTOR& cameraPos, Material*& material) const
{
	XMVECTOR viewDirection = XMVector3Normalize(cameraPos - fragPos);
	XMVECTOR lightDirection = -XMVector3Normalize(m_direction);
	XMVECTOR halfDir = XMVector3Normalize(lightDirection + viewDirection);

	XMVECTOR diffuse = max(XMVectorGetX(XMVector3Dot(fragNorm, lightDirection)), 0.0f) * material->albedo;

	//spec
	float spec = pow(max(XMVectorGetX(XMVector3Dot(fragNorm, halfDir)), 0.0f), material->glossines) * material->specular;
	XMVECTOR specular = XMVectorSet(spec, spec, spec, 0.0f);

	return m_lightColor * (diffuse + specular);
}

XMVECTOR Scene::FlashLight::illuminate(const XMVECTOR& fragPos, const XMVECTOR& fragNorm, const XMVECTOR& cameraPos, Material*& material) const
{
	XMVECTOR viewDirection = XMVector3Normalize(cameraPos - fragPos);
	XMVECTOR lightDirection = XMVector3Normalize(this->center - fragPos);
	float distance = XMVectorGetX(XMVector3Length(this->center - fragPos));

	XMVECTOR halfDir = XMVector3Normalize(lightDirection + viewDirection);

	//diff
	float attenuation = 1.0 / (m_quadraticIntens * distance * distance);

	XMVECTOR diffuse = max(XMVectorGetX(XMVector3Dot(fragNorm, lightDirection)), 0.0f) * material->albedo * attenuation;

	//spec
	float spec = pow(max(XMVectorGetX(XMVector3Dot(fragNorm, halfDir)), 0.0f), material->glossines) * material->specular;
	XMVECTOR specular = XMVectorSet(spec, spec, spec, 0.0f);

	// spotlight (soft edges)
	float theta = XMVectorGetX(XMVector3Dot(lightDirection, -XMVector3Normalize(m_direction)));
	float epsilon = (m_innerCutOff - m_outerCutOff);
	float intensity = (theta - m_outerCutOff) / epsilon;
	if (intensity < 0.0f)
		intensity = 0.0f;
	else if (intensity > 1.0f)
		intensity = 1.0f;


	XMVECTOR Li = m_lightColor / (m_linearIntens * distance);

	return  Li * intensity * (diffuse + specular);
}

Scene::PointLight::PointLight(XMVECTOR position, XMVECTOR color, float constInten, float linearInten, float quadInten)
{
	m_lightColor = color;

	this->center = position;
	this->radius = LIGHTMODEL_SIZE;
	
	this->material = Material(XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), 1.0f, 1.0f, XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f));

	m_constantIntens = constInten;
	m_linearIntens = linearInten;
	m_quadraticIntens = quadInten;

}

Scene::DirectionalLight::DirectionalLight(XMVECTOR direction, XMVECTOR color)
	: m_direction(direction), m_lightColor(color)
{
}

Scene::FlashLight::FlashLight(XMVECTOR position, XMVECTOR color, XMVECTOR direction, float innerCutOff, float outerCutOff,
	float constTntensity, float linIntensity, float quadrIntensity)
{
	m_direction = XMVector3Normalize(direction);

	m_lightColor = color;
	this->center = position;
	this->radius = LIGHTMODEL_SIZE;

	this->material = Material(XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), 1.0f, 1.0f, XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f));

	m_constantIntens = constTntensity;
	m_linearIntens = linIntensity;
	m_quadraticIntens = quadrIntensity;

	m_innerCutOff = innerCutOff;
	m_outerCutOff = outerCutOff;
}

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

Scene::FlashLightMover::FlashLightMover(FlashLight* pointLight, const XMVECTOR& pickedPosition)
	: flashLight(pointLight), pickedPosition(pickedPosition)
{
}

Scene::FlashLightMover::FlashLightMover(FlashLight* pointLight)
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
