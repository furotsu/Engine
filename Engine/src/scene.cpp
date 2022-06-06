#include "scene.h"
#include <math.h>
#include <algorithm>


XMVECTOR acesHdr2Ldr(const XMVECTOR& hdr)
{
	XMMATRIX m1 = XMMatrixSet(
		0.59719f, 0.07600f, 0.02840f, 0.0f,
		0.35458f, 0.90834f, 0.13383f, 0.0f,
		0.04823f, 0.01566f, 0.83777f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	);
	XMMATRIX m2 = XMMatrixSet(
		1.60475f, -0.10208, -0.00327f, 0.0f,
		-0.53108f, 1.10813, -0.07276f, 0.0f,
		-0.07367f, -0.00605, 1.07602f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	);
	XMVECTOR v = XMVector3Transform(hdr, m1);
	XMVECTOR a = v * (v + XMVectorSet(0.0245786f, 0.0245786f, 0.0245786f, 0.0f)) - XMVectorSet(0.000090537f, 0.000090537f, 0.000090537f, 0.0f);
	XMVECTOR b = v * (XMVectorSet(0.983729f, 0.983729f, 0.983729f, 0.0f) * v + XMVectorSet(0.4329510f, 0.4329510f, 0.4329510f, 0.0f)) + XMVectorSet(0.238081f, 0.238081f, 0.238081f, 0.0f);
	XMVECTOR ldr = math::clamp3(XMVector3Transform(a / b, m2), 0.0f, 1.0f);
	return ldr;
}

XMVECTOR adjustExposure(const XMVECTOR& color, float EV100)
{
	float LMax = (78.0f / (0.65f * 100.0f)) * powf(2.0f, EV100);
	return color * (1.0f / LMax);
}

XMVECTOR correctGamma(const XMVECTOR& color, float gamma)
{
	XMVECTOR g = XMVectorSet(gamma, gamma, gamma, 0.0f);
	return XMVectorPow(color, XMVectorDivide(XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), g));
}


// For each pixel determine what color it should 
//	   based on whether ray hit any object or not 
void Scene::computePixelColor(uint32_t posX, uint32_t posY, Window& win)
{
	ray r;
	r.origin = cameraPos;
	r.direction = -r.origin + (BL + (BR - BL) * (posX + 0.5f) / win.canvas.getWidth() + (TL - BL) * (win.canvas.getHeight() - posY + 0.5f) / win.canvas.getHeight());
	r.direction = XMVector3Normalize(XMVectorSet(XMVectorGetX(r.direction), XMVectorGetY(r.direction), XMVectorGetZ(r.direction), 0.0f));

	Material* material;
	Intersection hr;
	hr.reset();

	XMVECTOR col = illuminate(r, hr, material);

	col = adjustExposure(col, EV100);
	col = acesHdr2Ldr(col);
	col = correctGamma(col, 2.2f);

	win.canvas.setPixel(posX, posY, XMVectorGetX(col) * 255, XMVectorGetY(col) * 255, XMVectorGetZ(col) * 255);
}

XMVECTOR Scene::illuminate(ray& r, Intersection& hr, Material*& material)
{
	// set initial value as a sky color
	XMVECTOR color = m_ambientLight;
	if (findIntersection(r, hr, material))
	{
		color = (material->emmision + m_ambientLight) * material->albedo;

		if (XMVectorGetX(material->emmision) == 0.0f)
		{
			for (auto& l0 : m_pointLights)
				color += illuminate(hr, material, cameraPos, l0);

			for (auto& l1 : m_directionalLights)
				color += illuminate(hr, material, cameraPos, l1);

			for (auto& l2 : m_flashLights)
				color += illuminate(hr, material, cameraPos, l2);
		}
	}
	return color;
}

bool Scene::findIntersection(const math::ray& r, Intersection& outNearest, Material*& outMaterial)
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

bool Scene::findIntersectionShadow(const ray& r, Intersection& outNearest, Material*& outMaterial)
{
	bool foundIntersection = false;
	ObjRef ref = { nullptr, IntersectedType::NUM };

	foundIntersection |= m_surface.hit(r, ref, outNearest, outMaterial);

	for (auto& sphere : m_spheres)
		foundIntersection |= sphere.hit(r, ref, outNearest, outMaterial);

	for (auto& model : m_models)
		foundIntersection |= model.hit(r, ref, outNearest, outMaterial);

	return foundIntersection;
}

XMVECTOR Scene::illuminate(const Intersection& hr, Material*& material, const XMVECTOR& cameraPos, const PointLight& light, uint32_t depth)
{
	// adding small offset to avoid self-shadowing artifact
	ray r(hr.point + 1.0f * hr.normal, XMVector3Normalize(light.getCenter() - hr.point));

	Intersection hr2;
	Material* mat;
	hr2.reset();
	hr2.hitParam = XMVectorGetX(XMVector3Length(hr.point - light.getCenter())) - light.getRadius() - 1.0f;

	if (findIntersectionShadow(r, hr2, mat) && shadowsOn)
	{
		return XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	}

	XMVECTOR color = light.illuminate(hr.point, hr.normal, cameraPos, material);

	if (reflectionsOn && material->roughness < MAX_REFLECTION_ROUGHNESS && depth < MAX_REFLECTION_DEPTH)
	{
		r = ray(hr.point, XMVector3Normalize(XMVector3Reflect(hr.point - cameraPos, hr.normal)));
		hr2.reset();

		XMVECTOR colorRefl = illuminate(r, hr2, mat);
		return math::lerp(colorRefl, color, 1.0f / MAX_REFLECTION_ROUGHNESS * material->roughness);
	}
	
	return color;
}

XMVECTOR Scene::illuminate(const Intersection& hr, Material*& material, const XMVECTOR& cameraPos, const DirectionalLight& light)
{
	// adding small offset to avoid self-shadowing artifact
	ray r(hr.point + 0.01f * hr.normal, -light.m_direction);

	Intersection hr2;
	hr2.reset();
	Material* mat;

	if (findIntersectionShadow(r, hr2, mat) && shadowsOn)
	{
		return XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	}
	
	XMVECTOR color = light.illuminate(hr.point, hr.normal, cameraPos, material);

	if (reflectionsOn && material->roughness < MAX_REFLECTION_ROUGHNESS)
	{
		r = ray(hr.point, XMVector3Normalize(XMVector3Reflect(hr.point - cameraPos, hr.normal)));
		hr2.reset();

		// set initial value as a sky color
		XMVECTOR colorRefl = illuminate(r, hr2, mat);
		return math::lerp(colorRefl, color, 10.0f * material->roughness);
	}

	return color;
}

XMVECTOR Scene::illuminate(const Intersection& hr, Material*& material, const XMVECTOR& cameraPos, const SpotLight& light)
{
	// adding small offset to avoid self-shadowing artifact
	// adding small offset to avoid self-shadowing artifact
	ray r(hr.point + 1.0f * hr.normal, XMVector3Normalize(light.getCenter() - hr.point));

	Intersection hr2;
	Material* mat;
	hr2.reset();
	hr2.hitParam = XMVectorGetX(XMVector3Length(hr.point - light.getCenter())) - light.getRadius() - 1.0f;

	if (findIntersectionShadow(r, hr2, mat) && shadowsOn)
	{
		return XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	}

	XMVECTOR color = light.illuminate(hr.point, hr.normal, cameraPos, material);

	if (reflectionsOn && material->roughness < MAX_REFLECTION_ROUGHNESS)
	{
		r = ray(hr.point, XMVector3Normalize(XMVector3Reflect(hr.point - cameraPos, hr.normal)));
		hr2.reset();

		// set initial value as a sky color
		XMVECTOR colorRefl = illuminate(r, hr2, mat);
		return math::lerp(colorRefl, color, 10.0f * material->roughness);
	}

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
