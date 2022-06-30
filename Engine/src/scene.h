#pragma once

#define _USE_MATH_DEFINES

#include <windows.h>
#include <windowsx.h>

#include <vector>

#include "window.h"
#include "constants.h"
#include "plane.h"
#include "sphere.h"
#include "model.h"
#include "utility.h"
#include "camera.h"
#include "parallelExecutor.h"

using namespace math;


XMVECTOR frensel(float NdotL, const XMVECTOR& F0);
float smith(float rough2, float NoV, float NoL);
float ggx(float rough2, float NoH, float lightAngleSin = 0.0f, float lightAngleCos = 1.0f);

void branchlessONB(const XMVECTOR& n, XMVECTOR& b1, XMVECTOR& b2);
void fibonacciHemisphereDirection(uint32_t i, XMVECTOR& direction);

XMVECTOR acesHdr2Ldr(const XMVECTOR& hdr);
XMVECTOR adjustExposure(const XMVECTOR& color, float EV100);
XMVECTOR correctGamma(const XMVECTOR& color, float gamma);

float calcAttenuation(float radius, float distance);

XMVECTOR approximateClosestSphereDir(bool& intersects, XMVECTOR reflectionDir, float sphereCos, \
										XMVECTOR sphereRelPos, XMVECTOR sphereDir, float sphereDist, float sphereRadius);
void clampDirToHorizon(XMVECTOR& dir, float& NoD, XMVECTOR normal, float minNoD);
XMVECTOR CookTorrance_GGX(const XMVECTOR& viewDir, const XMVECTOR& fragNorm, float solidAngle, const XMVECTOR& lightCenter,
	const XMVECTOR& fragPos, const XMVECTOR& F0, float distance, float radius, float rough2);

class Scene
{
	// ---------- Utility types hidden from user code ----------
protected:
	// Light sources should be after models for easier comparison
	enum class IntersectedType {Sphere, Model, Surface, PointLight, FlashLight, NUM};

	struct ObjRef
	{
		void* object;
		IntersectedType type;
	};

public:

	// --------------------- Scene Objects ---------------------

	class Sphere : public math::Sphere
	{
	public:
		Material material;

		IntersectedType type;

		Sphere() = default;
		Sphere(XMVECTOR position, float radius, const Material& material);

		XMVECTOR position();

		bool hit(const math::ray& ray, ObjRef& outRef, math::Intersection& rec, Material*& outMaterial);

		friend class SphereMover;
	};

	class Model
	{
	public:
		Transform m_transformation;
		XMMATRIX m_modelMat;
		XMMATRIX m_modelInvMat;
		std::shared_ptr<Mesh> m_mesh;
		IntersectedType type;

		Material material;


		Model() = default;
		Model(XMVECTOR position, XMVECTOR scale, const std::shared_ptr<Mesh>& mesh, const Material& material = Material());

		XMVECTOR position();

		bool hit(math::ray r, ObjRef& outRef, math::Intersection& rec, Material*& outMaterial);

		friend class modelMover;
	};

	class Surface
	{
		math::Plane plane;
		Material material;

	public:
		Surface() = default;
		Surface(math::Plane plane, const Material& material);

		bool hit(const math::ray& r, ObjRef& outRef, math::Intersection& rec, Material*& outMaterial);
	};

	class PointLight : public Scene::Sphere
	{
	public:

		XMVECTOR m_lightColor;
		float m_lightPower;

		PointLight() = default;
		PointLight(XMVECTOR position, XMVECTOR color, float power = 1.0f, float radius = LIGHTMODEL_SIZE);

		XMVECTOR illuminate(const XMVECTOR& fragPos, const XMVECTOR& fragNorm, const XMVECTOR& cameraPos, Material*& albedo, bool reflOn = true) const;
	};

	class DirectionalLight
	{
	public:

		XMVECTOR m_lightColor;
		XMVECTOR m_direction;
		float m_lightPower;
		float solidAngle;

		DirectionalLight() = default;
		DirectionalLight(XMVECTOR direction, XMVECTOR color, float power = 1.0f, float solidAngle = 2.0f * M_PI);

		XMVECTOR illuminate(const XMVECTOR& fragPos, const XMVECTOR& fragNorm, const XMVECTOR& cameraPos, Material*& albedo) const;

	};

	class SpotLight : public Scene::Sphere
	{
	private:
	public:
		XMVECTOR m_lightColor;
		XMVECTOR m_direction;
		float m_lightPower;


		float m_innerCutOff;
		float m_outerCutOff;
		SpotLight() = default;
		SpotLight(XMVECTOR position, XMVECTOR color, XMVECTOR direction, float innerCutOff, float outerCutOff, float power = 1.0f, float radius = LIGHTMODEL_SIZE);

		XMVECTOR illuminate(const XMVECTOR& fragPos, const XMVECTOR& fragNorm, const XMVECTOR& cameraPos, Material*& material, bool reflOn = true) const;
	};

	// --------------------- Object Decorators ---------------------

	class IobjectMover
	{
	public:
		virtual void moveTo(const XMVECTOR&) = 0;
		virtual void moveBy(const XMVECTOR&) = 0;
		virtual XMVECTOR getPickedPos() = 0;

	};

	class SphereMover : public IobjectMover
	{
	public:
		Scene::Sphere* sphere;
		XMVECTOR pickedPosition;

		SphereMover() = default;
		SphereMover(Scene::Sphere*, const XMVECTOR&);
		SphereMover(Scene::Sphere*);

		virtual void moveTo(const XMVECTOR&) override;
		virtual void moveBy(const XMVECTOR&) override;
		virtual XMVECTOR getPickedPos()  override;
	};

	class ModelMover : public IobjectMover
	{
	public:
		Model* model;
		XMVECTOR pickedPosition;

		ModelMover() = default;
		ModelMover(Model*, const XMVECTOR&);
		ModelMover(Model*);

		virtual void moveTo(const XMVECTOR&) override;
		virtual void moveBy(const XMVECTOR&) override;
		virtual XMVECTOR getPickedPos() override;
	};

	class PointLightMover : public IobjectMover
	{
	public:
		PointLight* pointLight;
		XMVECTOR pickedPosition;

		PointLightMover() = default;
		PointLightMover(PointLight*, const XMVECTOR&);
		PointLightMover(PointLight*);

		virtual void moveTo(const XMVECTOR&) override;
		virtual void moveBy(const XMVECTOR&) override;
		virtual XMVECTOR getPickedPos() override;
	};

	class FlashLightMover : public IobjectMover
	{
	public:
		SpotLight* flashLight;
		XMVECTOR pickedPosition;

		FlashLightMover() = default;
		FlashLightMover(SpotLight*, const XMVECTOR&);
		FlashLightMover(SpotLight*);

		virtual void moveTo(const XMVECTOR&) override;
		virtual void moveBy(const XMVECTOR&) override;
		virtual XMVECTOR getPickedPos() override;
	};

	struct IntersectionQuery
	{
		Intersection intersection;
		Material* materialPtr;
		float distToPickedObj;

		std::unique_ptr<Scene::IobjectMover> mover;
	};



public:

	Surface m_surface;
	std::vector<Model> m_models;
	std::vector<Sphere> m_spheres;
	float EV100;

	XMVECTOR TL;
	XMVECTOR TR;
	XMVECTOR BR;
	XMVECTOR BL;
	XMVECTOR cameraPos;

	XMVECTOR m_ambientLight;
	std::vector<PointLight> m_pointLights;
	std::vector<DirectionalLight> m_directionalLights;
	std::vector<SpotLight> m_flashLights;

	bool globalIlluminationOn;
	bool reflectionsOn;
	bool shadowsOn;

	Scene() = default;

	void computePixelColor(uint32_t posX, uint32_t posY, Window& win);

	bool findIntersection(const ray& r, IntersectionQuery& query);
	
	XMVECTOR illuminate(ray& r, uint32_t depth = 1u);
	XMVECTOR illuminateIndirect(const Intersection& hr, const XMVECTOR& cameraPos, uint32_t depth);

	void render(Window& w, Camera& camera, ParallelExecutor& executor);

	void addSphere(const Sphere& sphereModel);
	void addModel(Model model);
	void setSurface(Surface surface);

	void setAmbient(XMVECTOR ambient);
	void addPointLight(const PointLight& light);
	void addDirLight(const DirectionalLight& light);
	void addFlashLight(const SpotLight& light);

	void pickObject(const Camera& camera, const XMVECTOR&, IntersectionQuery&);

protected:
	XMVECTOR illuminate(const Intersection& hr, Material*& material, const XMVECTOR& cameraPos, const PointLight& light);
	XMVECTOR illuminate(const Intersection& hr, Material*& material, const XMVECTOR& cameraPos, const DirectionalLight& light);
	XMVECTOR illuminate(const Intersection& hr, Material*& material, const XMVECTOR& cameraPos, const SpotLight& light);

	void findIntersectionInternal(const ray& r, ObjRef& outRef, Intersection& outNearest, Material*& outMaterial);
	void findIntersectionShadow(const ray& r, ObjRef& outRef, Intersection& outNearest, Material*& outMaterial);

};


