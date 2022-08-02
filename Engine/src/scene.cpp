#include <math.h>
#include <algorithm>
#include <cmath>

#include "d3d.hpp"
#include "scene.hpp"
#include "debug.hpp"

namespace engine
{
	void Scene::init(std::vector<ShaderInfo>& shaders, std::vector<D3D11_INPUT_ELEMENT_DESC>& ied)
	{
		models.init(ShaderManager::GetInstance()->getShader(shaders, ied));
	}

	void Scene::addModel(std::string filepath, std::string filename, std::vector<XMFLOAT3> positions)
	{
		models.addModel(filepath, filename, positions);
	}

	void Scene::setSkybox(Sky skybox, std::vector<ShaderInfo> shaders)
	{
		this->skybox = skybox;
		this->skybox.init(shaders);
	}

	bool Scene::findIntersection(const ray& r, IntersectionQuery& query)
	{
		ObjRef ref = { nullptr, IntersectedType::NUM };

		findIntersectionInternal(r, ref, query.intersection, query.materialPtr);
		switch (ref.type)
		{
		}
		return ref.type != IntersectedType::NUM;
	}

	void Scene::renderFrame(Window& window, const Camera& camera)
	{
		ALWAYS_ASSERT(window.m_renderTargetView != NULL && "render target is NULL");
		s_devcon->OMSetRenderTargets(1, window.m_renderTargetView.access(), window.m_pDSV); 

		FLOAT backgroundColor[4]{ 0.0f, 0.2f, 0.4f, 1.0f };
		s_devcon->ClearRenderTargetView(window.m_renderTargetView.ptr(), backgroundColor);
		s_devcon->ClearDepthStencilView(window.m_pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 0.0f, 0);

		models.render();
		skybox.render(window, camera);

		window.m_swapchain->Present(0, 0);
	}

	void Scene::clean()
	{
		models.clean();
		skybox.clean();
	}

	void Scene::findIntersectionInternal(const ray& r, ObjRef& outRef, Intersection& outNearest, Material*& outMaterial)
	{
	}

	void Scene::findIntersectionShadow(const ray& r, ObjRef& outRef, Intersection& outNearest, Material*& outMaterial)
	{
	}
}