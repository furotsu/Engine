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

	// set initial value as a sky color
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
		SpotLight* model = static_cast<SpotLight*>(ref.object);
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

XMVECTOR Scene::illuminate(const Intersection& hr, Material*& material, const XMVECTOR& cameraPos, const SpotLight& light)
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
void Scene::addFlashLight(const SpotLight& light) { m_flashLights.push_back(light); }

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
