#pragma once

#define _USE_MATH_DEFINES

#include <vector>

#include "window.hpp"
#include "constants.hpp"
#include "plane.hpp"
#include "sphere.hpp"
#include "model.hpp"
#include "utility.hpp"
#include "parallelExecutor.hpp"
#include "camera.hpp"
#include "sky.hpp"
#include "shaderManager.hpp"


using namespace math;

namespace engine
{
	class Scene
	{
		// ---------- Utility types hidden from user code ----------
	protected:
		// Light sources should be after models for easier comparison
		enum class IntersectedType { Sphere, Model, Surface, PointLight, FlashLight, NUM };

		struct ObjRef
		{
			void* object;
			IntersectedType type;
		};

	public:

		// --------------------- Object Decorators ---------------------

		class IobjectMover
		{
		public:
			virtual void moveTo(const XMVECTOR&) = 0;
			virtual void moveBy(const XMVECTOR&) = 0;
			virtual XMVECTOR getPickedPos() = 0;

		};

		struct IntersectionQuery
		{
			Intersection intersection;
			Material* materialPtr;
			float distToPickedObj;

			std::unique_ptr<Scene::IobjectMover> mover;
		};

	public:

		std::vector<Model> m_models;

		Model model;
		Sky skybox;
		std::shared_ptr<ShaderProgram> modelProgram;
		std::shared_ptr<Mesh> cubeMesh;

		Scene() = default;

		void init();

		void addModel(Model model, std::vector<ShaderInfo> shaders, std::vector<D3D11_INPUT_ELEMENT_DESC> ied);
		void setSkybox(Sky skybox, std::vector<ShaderInfo> shaders);

		bool findIntersection(const ray& r, IntersectionQuery& query);

		void renderFrame(Window& window, const Camera& camera);
		void renderCube(Window& window, const Camera& camera);

		void clean();

	protected:

		void findIntersectionInternal(const ray& r, ObjRef& outRef, Intersection& outNearest, Material*& outMaterial);
		void findIntersectionShadow(const ray& r, ObjRef& outRef, Intersection& outNearest, Material*& outMaterial);
	};


}