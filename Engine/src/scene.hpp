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
#include "opaque_instances.hpp"


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

		DxResPtr<ID3D11Texture2D> m_pDepthStencil;
		DxResPtr<ID3D11DepthStencilState> m_pDSState;
	public:

		Sky skybox;

		DxResPtr<ID3D11DepthStencilView> m_pDSV;

		Scene() = default;

		void init(const Window& window);
		void addModel(std::shared_ptr<Model> model, std::vector<OpaqueInstances::Instance> position = {XMMATRIX()});
		void setSkybox(Sky skybox, std::vector<ShaderInfo> shaders);

		bool findIntersection(const ray& r, IntersectionQuery& query);

		void renderFrame(Window& window, const Camera& camera);

		void onResize(const Window& window);
		void bindDepthStencil();

		void clean();

	protected:

		void findIntersectionInternal(const ray& r, ObjRef& outRef, Intersection& outNearest, Material*& outMaterial);
		void findIntersectionShadow(const ray& r, ObjRef& outRef, Intersection& outNearest, Material*& outMaterial);

		void createDepthStencilBuffer(uint32_t width, uint32_t height);
	};


}