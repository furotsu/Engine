
#include <math.h>
#include <algorithm>
#include <cmath>

#include "scene.h"


// For each pixel determine what color it should 
//	   based on whether ray hit any object or not 
void Scene::computePixelColor(uint32_t posX, uint32_t posY, Window& win)
{
	ray r;
	r.origin = cameraPos;
	r.direction = -r.origin + (BL + (BR - BL) * (posX + 0.5f) / win.canvas.getWidth() + (TL - BL) * (win.canvas.getHeight() - posY + 0.5f) / win.canvas.getHeight());
	r.direction = XMVector3Normalize(XMVectorSet(XMVectorGetX(r.direction), XMVectorGetY(r.direction), XMVectorGetZ(r.direction), 0.0f));

	XMVECTOR col = illuminate(r);

	col = adjustExposure(col, EV100);
	col = acesHdr2Ldr(col);
	col = correctGamma(col, 2.2f);

	win.canvas.setPixel(posX, posY, XMVectorGetX(col) * 255, XMVectorGetY(col) * 255, XMVectorGetZ(col) * 255);
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
		query.distToPickedObj = query.intersection.hitParam;
	}break;
	case IntersectedType::Model:
	{
		Model* model = static_cast<Model*>(ref.object);
		query.mover.reset(new ModelMover(model, query.intersection.point));
		query.distToPickedObj = query.intersection.hitParam;
	}break;
	case IntersectedType::PointLight:
	{
		PointLight* model = static_cast<PointLight*>(ref.object);
		query.mover.reset(new PointLightMover(model, query.intersection.point));
		query.distToPickedObj = query.intersection.hitParam;
	}break;
	case IntersectedType::FlashLight:
	{
		SpotLight* model = static_cast<SpotLight*>(ref.object);
		query.mover.reset(new FlashLightMover(model, query.intersection.point));
		query.distToPickedObj = query.intersection.hitParam;
	}break;
	}
	return ref.type != IntersectedType::NUM;
}

void Scene::findIntersectionInternal(const ray& r, ObjRef& outRef, Intersection& outNearest, Material*& outMaterial)
{
	m_surface.hit(r, outRef, outNearest, outMaterial);

	for (auto& sphere : m_spheres)
		sphere.hit(r, outRef, outNearest, outMaterial);

	for (auto& model : m_models)
		model.hit(r, outRef, outNearest, outMaterial);

	for (auto& light : m_pointLights)
		light.hit(r, outRef, outNearest, outMaterial);

	for (auto& light : m_flashLights)
		light.hit(r, outRef, outNearest, outMaterial);
}

void Scene::findIntersectionShadow(const ray& r, ObjRef& outRef, Intersection& outNearest, Material*& outMaterial)
{
	m_surface.hit(r, outRef, outNearest, outMaterial);

	for (auto& sphere : m_spheres)
		sphere.hit(r, outRef, outNearest, outMaterial);

	for (auto& model : m_models)
		model.hit(r, outRef, outNearest, outMaterial);
}

XMVECTOR Scene::illuminate(ray& r, uint32_t depth)
{
	// set initial value as a sky color
	XMVECTOR color = m_ambientLight;

	Material* material;
	Intersection hr;
	hr.reset();

	bool interFound = false;
	ObjRef ref = { nullptr, IntersectedType::NUM };

	if (depth == 1u)
		findIntersectionInternal(r, ref, hr, material);
	else
		findIntersectionShadow(r, ref, hr, material);

	if (ref.type != IntersectedType::NUM)
	{

		if (globalIlluminationOn && depth == 1u)
		{
			//calculate indirect light value from hemisphere instead of using ambient constant value
			color = (material->emmision + illuminateIndirect(hr, cameraPos, depth)) * material->albedo;
		}
		else
		{
			color = (material->emmision + m_ambientLight) * material->albedo;
		}

		if (ref.type < IntersectedType::PointLight)
		{
			for (auto& l0 : m_pointLights)
				color += illuminate(hr, material, cameraPos, l0);

			for (auto& l1 : m_directionalLights)
				color += illuminate(hr, material, cameraPos, l1);

			for (auto& l2 : m_flashLights)
				color += illuminate(hr, material, cameraPos, l2);

			//reflections
			if (reflectionsOn && material->roughness < MAX_REFLECTION_ROUGHNESS && depth < MAX_REFLECTION_DEPTH)
			{
				r = ray(hr.point, XMVector3Normalize(XMVector3Reflect(hr.point - cameraPos, hr.normal)));

				XMVECTOR halfDir = XMVector3Normalize(r.direction + (hr.point - cameraPos));
				XMVECTOR F = math::clamp3(frensel(XMVectorGetX(XMVector3Dot(hr.normal, r.direction)), material->F0), 0.0f, 1.0f);

				XMVECTOR colorRefl = illuminate(r, ++depth) * F;
				color = (XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f) - F) * color;

				return colorRefl + color;
			}
		}
	}
	return color;
}

XMVECTOR Scene::illuminateIndirect(const Intersection& hr, const XMVECTOR& cameraPos, uint32_t depth)
{
	XMVECTOR color = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR xAxis = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR yAxis = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//should be normalized
	XMVECTOR zAxis = hr.normal;
	branchlessONB(zAxis, xAxis, yAxis);
	//matrix to convert hemisphere coordinates for coords where fragment normal will be z-part of basis matrix
	XMMATRIX transformMat = XMMatrixTranspose({ (XMVectorGetZ(zAxis) < 0.0f) ? xAxis : -xAxis, -yAxis, zAxis, {0.0f, 0.0f, 0.0f, 0.0f} });

	for (uint32_t i = 0; i != RAYS_ABOVE_HEMISPHERE_COUNT; i++)
	{
		XMVECTOR direction;
		fibonacciHemisphereDirection(i, direction);
		direction = XMVector3Transform(direction, transformMat);

		ray sampleRay(hr.point, XMVector3Normalize(direction));
	
		color += illuminate(sampleRay, ++depth);
	}

	color = XMVectorScale(color, 2.0f * M_PI / static_cast<float>(RAYS_ABOVE_HEMISPHERE_COUNT));
	return color;
}

XMVECTOR Scene::illuminate(const Intersection& hr, Material*& material, const XMVECTOR& cameraPos, const PointLight& light)
{
	// adding small offset to avoid self-shadowing artifact
	ray r(hr.point + SMALL_OFFSET * hr.normal, XMVector3Normalize(light.getCenter() - hr.point));
	
	Intersection hr2;
	Material* mat;
	hr2.reset();
	hr2.hitParam = XMVectorGetX(XMVector3Length(hr.point - light.getCenter())) - light.getRadius() - SMALL_OFFSET;

	ObjRef ref = { nullptr, IntersectedType::NUM };
	findIntersectionShadow(r, ref, hr2, mat);

	if ((ref.type < IntersectedType::PointLight) && shadowsOn)
	{
			return XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	}

	XMVECTOR color = light.illuminate(hr.point, hr.normal, cameraPos, material, reflectionsOn);

	return color;
}

XMVECTOR Scene::illuminate(const Intersection& hr, Material*& material, const XMVECTOR& cameraPos, const DirectionalLight& light)
{
	// adding small offset to avoid self-shadowing artifact
	ray r(hr.point + SMALL_OFFSET * hr.normal, -light.m_direction);

	Intersection hr2;
	hr2.reset();
	Material* mat;

	ObjRef ref = { nullptr, IntersectedType::NUM };
	findIntersectionShadow(r, ref, hr2, mat);

	if (ref.type < IntersectedType::PointLight && shadowsOn)
	{
		return XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	}
	
	XMVECTOR color = light.illuminate(hr.point, hr.normal, cameraPos, material);

	return color;
}

XMVECTOR Scene::illuminate(const Intersection& hr, Material*& material, const XMVECTOR& cameraPos, const SpotLight& light)
{
	// adding small offset to avoid self-shadowing artifact
	ray r(hr.point + SMALL_OFFSET * hr.normal, XMVector3Normalize(light.getCenter() - hr.point));

	Intersection hr2;
	Material* mat;
	hr2.reset();
	hr2.hitParam = XMVectorGetX(XMVector3Length(hr.point - light.getCenter())) - light.getRadius() - SMALL_OFFSET;

	ObjRef ref = { nullptr, IntersectedType::NUM };
	findIntersectionShadow(r, ref, hr2, mat);

	if ((ref.type < IntersectedType::PointLight) && shadowsOn)
	{
		return XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	}

	XMVECTOR color = light.illuminate(hr.point, hr.normal, cameraPos, material, reflectionsOn);

	return color;
}

void Scene::render(Window& window, Camera& camera, ParallelExecutor& executor)
{
	ray r;
	XMVECTOR point;

	cameraPos = camera.position();
	
	TL = XMVectorSet(-1.0f, 1.0f, 1.0f, 1.0f);
	TR = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	BR = XMVectorSet(1.0f, -1.0f, 1.0f, 1.0f);
	BL = XMVectorSet(-1.0f, -1.0f, 1.0f, 1.0f);

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

	auto func = [this, &window](uint32_t threadIndex, uint32_t taskIndex) { computePixelColor(taskIndex % window.canvas.getWidth(), taskIndex / window.canvas.getWidth(), window); };
	executor.execute(func, window.canvas.getWidth() * window.canvas.getHeight(), 20);
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

void Scene::pickObject(const Camera& camera, const XMVECTOR& mousePos, IntersectionQuery& queryOut)
{
	XMVECTOR point = XMVector4Transform(mousePos, camera.m_projInv);
	point /= XMVectorGetW(point);
	point = XMVector4Transform(point, camera.m_viewInv);

	ray r(camera.position(), point);
	r.direction = -r.origin + point;
	r.direction = XMVector3Normalize(XMVectorSet(XMVectorGetX(r.direction), XMVectorGetY(r.direction), XMVectorGetZ(r.direction), 0.0f));

	findIntersection(r, queryOut);
}
