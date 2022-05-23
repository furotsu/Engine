#include "scene.h"


// For each pixel determine what color it should 
//	   based on whether ray hit any object or not 
XMVECTOR Scene::getPixelColor(const ray& r)
{
	Intersection hr;
	hr.reset();

	XMVECTOR resColor = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	if (findIntersectionObjects(r, hr))
	{

		for (auto& l0 : m_pointLights)
			resColor += illuminate(hr, l0.first);

		for (auto& l1 : m_directionalLights)
			resColor += illuminate(hr, l1);

		for (auto& l2 : m_flashLights)
			resColor += illuminate(hr, l2.first);
	}
	
	if (findIntersectionLights(r, hr))
		resColor = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);

	return resColor;
}

bool Scene::findIntersectionObjects(const ray& r, Intersection& hr)
{
	bool foundIntersection = false;
	hr.reset();

	foundIntersection |= m_surface.hit(r, hr);

	for (auto& sphere : m_spheres)
		foundIntersection |= sphere.hit(r, hr);

	for (auto& model : m_models)
		foundIntersection |= model.hit(r, hr);

	return foundIntersection;
}

bool Scene::findIntersectionLights(const ray& r, Intersection& hr)
{
	bool foundIntersection = false;

	for (auto& light : m_pointLights)
		foundIntersection |= light.second.hit(r, hr);

	for (auto& light : m_flashLights)
		foundIntersection |= light.second.hit(r, hr);

	return foundIntersection;
}

XMVECTOR Scene::illuminate(const Intersection& hr, const PointLight& light)
{
	// adding small offset to avoid self-shadowing artifact
	ray r(hr.point + 0.01f * hr.normal, XMVector3Normalize(light.m_position - hr.point));

	Intersection tmp;
	tmp.reset();

	if (findIntersectionObjects(r, tmp))
	{
		return XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	}
	else
	{
		return light.illuminate(hr.point, hr.normal, hr.material);
	}
}

XMVECTOR Scene::illuminate(const Intersection& hr, const DirectionalLight& light)
{
	// adding small offset to avoid self-shadowing artifact
	ray r(hr.point + 0.0001f * hr.normal, -light.m_direction);

	Intersection tmp;
	tmp.reset();

	if (findIntersectionObjects(r, tmp))
	{
		return XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	}
	else
	{
		return light.illuminate(hr.point, hr.normal, hr.material);
	}
}

XMVECTOR Scene::illuminate(const Intersection& hr, const FlashLight& light)
{
	// adding small offset to avoid self-shadowing artifact
	ray r(hr.point + 0.0001f * hr.normal, XMVector3Normalize(light.m_position - light.m_direction));

	Intersection tmp;
	tmp.reset();

	XMVECTOR res = light.illuminate(hr.point, hr.normal, hr.material);

	if (findIntersectionObjects(r, tmp))
	{
		return  XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	}
	else
	{
		return res;
	}
}

void Scene::render(Window& window, XMMATRIX matrix, Camera& camera)
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
			r.direction = XMVector3Normalize(BL + (BR - BL) * w/window.canvas.getWidth() + (TL - BL) * (window.canvas.getHeight() - h) / window.canvas.getHeight());

			XMVECTOR col = getPixelColor(r);

			float r = XMVectorGetX(col) < 1.0f ? XMVectorGetX(col) : 0.99f;
			float g = XMVectorGetY(col) < 1.0f ? XMVectorGetY(col) : 0.99f;
			float b = XMVectorGetZ(col) < 1.0f ? XMVectorGetZ(col) : 0.99f;
			
			window.canvas.setPixel(w, h, r * 255, g * 255, b * 255);
		}
	}
}

void Scene::updateLightsPos()
{
	for (auto& l1 : m_pointLights)
		l1.first.m_position = l1.second.position();

	for (auto& l2 : m_flashLights)
		l2.first.m_position = l2.second.position();
}

void Scene::addSphere(const SphereModel& sphereModel)
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

void Scene::addPointLight(PointLight light, SphereModel sphereModel)
{
	m_pointLights.push_back({ light, sphereModel });
}
void Scene::addDirLight(const DirectionalLight& light) { m_directionalLights.push_back(light); }
void Scene::addFlashLight(const FlashLight& light, SphereModel sphereModel) { m_flashLights.push_back({ light, sphereModel }); }


void Scene::pickObject(const Camera& camera, const XMVECTOR& mousePos)
{

	float x = (2.0f * XMVectorGetX(mousePos)) / 400.0f - 1.0f;
	float y = 1.0f - (2.0f * XMVectorGetY(mousePos)) / 200.0f;

	ray r(camera.position(), XMVector4Transform(XMVectorSet(x, y, 1.0f, 1.0f), camera.m_viewProjInv));

	Intersection hr;
	hr.reset();

	for (auto& sphere : m_spheres)
	{
		if (sphere.hit(r, hr))
		{
			objPicked = true;
			m_movingPlane = Plane(-camera.forward(), hr.point);
			pickedObjPtr = std::make_unique<SphereMover>(sphere, hr.point);
		}
	}	
	for (auto& model : m_models)
	{
		if (model.hit(r, hr))
		{
			objPicked = true;
			m_movingPlane = Plane(-camera.forward(), hr.point);
			pickedObjPtr = std::make_unique<ModelMover>(model, hr.point);
		}
	}
	for (auto& light : m_pointLights)
	{
		if (light.second.hit(r, hr))
		{
			objPicked = true;
			m_movingPlane = Plane(-camera.forward(), hr.point);
			std::cout << "actual picked position: " << std::endl;
			std::cout << XMVectorGetX(hr.point) << " " << XMVectorGetY(hr.point) << " " << XMVectorGetZ(hr.point) << std::endl;

			pickedObjPtr = std::make_unique<SphereMover>(light.second, hr.point);
		}
	}
	for (auto& light : m_flashLights)
	{
		if (light.second.hit(r, hr))
		{
			objPicked = true;
			m_movingPlane = Plane(-camera.forward(), hr.point);
			pickedObjPtr = std::make_unique<SphereMover>(light.second, hr.point);
		}
	}
}