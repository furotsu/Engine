#pragma once

#include <windows.h>
#include <windowsx.h>

#include <vector>

#include "window.h"

#include "plane.h"
#include "sphere.h"
#include "model.h"
#include "utility.h"
#include "camera.h"
#include "parallelExecutor.h"

using namespace math;

constexpr float LIGHTMODEL_SIZE = 5.0f;
constexpr uint32_t MAX_REFLECTION_DEPTH = 3u;
constexpr float MAX_REFLECTION_ROUGHNESS = 0.12f;
constexpr float REFLECTION_ROUGNESS_MULTIPLIER = 1.0f / MAX_REFLECTION_ROUGHNESS;

XMVECTOR frensel(float NdotL, const XMVECTOR& F0);
float smith(float rough2, float NoV, float NoL);
float ggx(float rough2, float NoH, float lightAngleSin = 0.0f, float lightAngleCos = 1.0f);

XMVECTOR acesHdr2Ldr(const XMVECTOR& hdr);
XMVECTOR adjustExposure(const XMVECTOR& color, float EV100);
XMVECTOR correctGamma(const XMVECTOR& color, float gamma);

class Scene
{
	// ---------- Utility types hidden from user code ----------
protected:
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

		DirectX::XMVECTOR m_lightColor;
		float m_lightPower;

		PointLight() = default;
		PointLight(DirectX::XMVECTOR position, DirectX::XMVECTOR color, float power = 1.0f, float radius = LIGHTMODEL_SIZE);

		DirectX::XMVECTOR illuminate(const DirectX::XMVECTOR& fragPos, const DirectX::XMVECTOR& fragNorm, const XMVECTOR& cameraPos, Material*& albedo) const;
	};

	class DirectionalLight
	{
	public:

		XMVECTOR m_lightColor;
		XMVECTOR m_direction;
		float m_lightPower;

		DirectionalLight() = default;
		DirectionalLight(DirectX::XMVECTOR direction, DirectX::XMVECTOR color, float power = 1.0f);

		DirectX::XMVECTOR illuminate(const DirectX::XMVECTOR& fragPos, const DirectX::XMVECTOR& fragNorm, const XMVECTOR& cameraPos, Material*& albedo) const;

	};

	class SpotLight : public Scene::Sphere
	{
	private:
	public:
		DirectX::XMVECTOR m_lightColor;
		DirectX::XMVECTOR m_direction;
		float m_lightPower;


		float m_innerCutOff;
		float m_outerCutOff;
		SpotLight() = default;
		SpotLight(DirectX::XMVECTOR position, DirectX::XMVECTOR color, DirectX::XMVECTOR direction, float innerCutOff, float outerCutOff, float power = 1.0f, float radius = LIGHTMODEL_SIZE);

		DirectX::XMVECTOR illuminate(const DirectX::XMVECTOR& fragPos, const DirectX::XMVECTOR& fragNorm, const XMVECTOR& cameraPos, Material*& material) const;
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

	bool reflectionsOn;
	bool shadowsOn;

	Scene() = default;

	void computePixelColor(uint32_t posX, uint32_t posY, Window& win);

	bool findIntersection(const math::ray& r, Intersection& outNearest, Material*& outMaterial);
	bool findIntersection(const ray& r, IntersectionQuery& query);

	
	XMVECTOR illuminate(ray& r, Intersection& hr, Material*& material, uint32_t depth = 1u);

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
	XMVECTOR illuminate(const Intersection& hr, Material*& material, const XMVECTOR& cameraPos, const PointLight& light, uint32_t depth = 1u);
	XMVECTOR illuminate(const Intersection& hr, Material*& material, const XMVECTOR& cameraPos, const DirectionalLight& light, uint32_t depth = 1u);
	XMVECTOR illuminate(const Intersection& hr, Material*& material, const XMVECTOR& cameraPos, const SpotLight& light, uint32_t depth = 1u);

	void findIntersectionInternal(const ray& r, ObjRef& outRef, Intersection& outNearest, Material*& outMaterial);
	bool findIntersectionShadow(const ray& r, Intersection& outNearest, Material*& outMaterial);

};


