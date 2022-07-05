#pragma once

#define _USE_MATH_DEFINES

#include <vector>

#include "window.hpp"
#include "constants.hpp"
#include "plane.hpp"
#include "sphere.hpp"
#include "mesh.hpp"
#include "utility.hpp"
#include "parallelExecutor.hpp"

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

		// --------------------- Scene Objects ---------------------

		class Model
		{
			DxResPtr<ID3D11Buffer> m_pVBuffer;                // the pointer to the vertex buffer
			std::shared_ptr<Mesh> m_mesh;
		public:

			Model() = default;

			void init(const std::shared_ptr<Mesh>& mesh);
			void initBuffers();

			friend class Scene;
		};

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

		Scene() = default;

		bool findIntersection(const ray& r, IntersectionQuery& query);

		void renderFrame(Window& window, Model& model);

	protected:

		void findIntersectionInternal(const ray& r, ObjRef& outRef, Intersection& outNearest, Material*& outMaterial);
		void findIntersectionShadow(const ray& r, ObjRef& outRef, Intersection& outNearest, Material*& outMaterial);

	};


}