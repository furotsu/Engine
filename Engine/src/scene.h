#pragma once

#include <windows.h>
#include <windowsx.h>

#include <vector>

#include "window.h"

#include "surface.h"
#include "sphere.h"
#include "model.h"
#include "objectMover.h"

#include "camera.h"

using namespace math;

class Scene
{
	
public:

	Surface m_surface;
	Plane m_movingPlane;
	std::vector<Model> m_models;
	std::vector<SphereModel> m_spheres;

	std::unique_ptr<IobjectMover> pickedObjPtr;
	bool objPicked;

	std::vector<std::pair<PointLight, SphereModel>> m_pointLights;
	std::vector<DirectionalLight> m_directionalLights;
	std::vector<std::pair<FlashLight, SphereModel>> m_flashLights;


	Scene() = default;

	XMVECTOR getPixelColor(const ray& r);

	bool findIntersectionObjects(const ray& r, Intersection& hr);
	bool findIntersectionLights(const ray& r, Intersection& hr);
	XMVECTOR illuminate(const Intersection& hr, const PointLight& light);
	XMVECTOR illuminate(const Intersection& hr, const DirectionalLight& light);
	XMVECTOR illuminate(const Intersection& hr, const FlashLight& light);

	void render(Window& w, XMMATRIX, Camera& camera);

	void updateLightsPos();

	void addSphere(const SphereModel& sphereModel);
	void addModel(Model model);
	void setSurface(Surface surface);
	void addPointLight(PointLight light, SphereModel sphereModel);
	void addDirLight(const DirectionalLight& light);
	void addFlashLight(const FlashLight& light, SphereModel sphereModel);

	void pickObject(const Camera& camera, const XMVECTOR&);
};


