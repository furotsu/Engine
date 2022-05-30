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

using namespace math;

constexpr float LIGHTMODEL_SIZE = 10.0f;

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

		Material material;


		Model() = default;
		Model(XMVECTOR position, XMVECTOR scale, const std::shared_ptr<Mesh>& mesh, const Material& material = Material());

		XMVECTOR position();

		bool hit(math::ray r, ObjRef& outRef, math::Intersection& rec, Material*& outMaterial);

		friend class modelMover;
	};

	class Surface
	{
		XMMATRIX  m_modelMat;
		XMMATRIX  m_modelInvMat;

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

		float m_constantIntens;
		float m_linearIntens;
		float m_quadraticIntens;

		PointLight() = default;
		PointLight(DirectX::XMVECTOR position, DirectX::XMVECTOR color, float constInten = 1.0f, float linearInten = 0.014f, float quadInten = 0.007f);

		DirectX::XMVECTOR illuminate(const DirectX::XMVECTOR& fragPos, const DirectX::XMVECTOR& fragNorm, const XMVECTOR& cameraPos, Material*& albedo) const;
	};

	class DirectionalLight
	{
	public:

		XMVECTOR m_lightColor;
		XMVECTOR m_direction;

		DirectionalLight() = default;
		DirectionalLight(DirectX::XMVECTOR direction, DirectX::XMVECTOR color);

		DirectX::XMVECTOR illuminate(const DirectX::XMVECTOR& fragPos, const DirectX::XMVECTOR& fragNorm, const XMVECTOR& cameraPos, Material*& albedo) const;

	};

	class FlashLight : public Scene::Sphere
	{
	private:
	public:
		DirectX::XMVECTOR m_lightColor;
		DirectX::XMVECTOR m_direction;

		float m_constantIntens;
		float m_linearIntens;
		float m_quadraticIntens;

		float m_innerCutOff;
		float m_outerCutOff;
		FlashLight() = default;
		FlashLight(DirectX::XMVECTOR position, DirectX::XMVECTOR color, DirectX::XMVECTOR direction, float innerCutOff, float outerCutOff,
			float constTntensity = 1.0f, float linearInten = 0.034f, float quadInten = 0.03f);

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
		FlashLight* flashLight;
		XMVECTOR pickedPosition;

		FlashLightMover() = default;
		FlashLightMover(FlashLight*, const XMVECTOR&);
		FlashLightMover(FlashLight*);

		virtual void moveTo(const XMVECTOR&) override;
		virtual void moveBy(const XMVECTOR&) override;
		virtual XMVECTOR getPickedPos() override;
	};

	struct IntersectionQuery
	{
		Intersection intersection;
		Material* materialPtr;

		std::unique_ptr<Scene::IobjectMover> mover;
	};
public:

	Surface m_surface;
	std::vector<Model> m_models;
	std::vector<Sphere> m_spheres;

	XMVECTOR tmp;

	IntersectionQuery pickedObjMoverQuery;
	float distToPickedObj;

	XMVECTOR m_ambientLight;
	std::vector<PointLight> m_pointLights;
	std::vector<DirectionalLight> m_directionalLights;
	std::vector<FlashLight> m_flashLights;

	Scene() = default;

	XMVECTOR getPixelColor(const ray& r, const XMVECTOR& cameraPos);

	bool findIntersection(const math::ray& r, Intersection& outNearest,  Material*& outMaterial);
	bool findIntersection(const ray& r, IntersectionQuery& query);

	XMVECTOR illuminate(const Intersection& hr, Material*& material, const XMVECTOR& cameraPos, const PointLight& light);
	XMVECTOR illuminate(const Intersection& hr, Material*& material, const XMVECTOR& cameraPos, const DirectionalLight& light);
	XMVECTOR illuminate(const Intersection& hr, Material*& material, const XMVECTOR& cameraPos, const FlashLight& light);

	void render(Window& w, Camera& camera);

	void addSphere(const Sphere& sphereModel);
	void addModel(Model model);
	void setSurface(Surface surface);

	void setAmbient(XMVECTOR ambient);
	void addPointLight(const PointLight& light);
	void addDirLight(const DirectionalLight& light);
	void addFlashLight(const FlashLight& light);

	void pickObject(const Camera& camera, const XMVECTOR&);

protected:
	void findIntersectionInternal(const ray& r, ObjRef& outRef, Intersection& outNearest, Material*& outMaterial);
};


