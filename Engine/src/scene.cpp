#include <math.h>
#include <algorithm>
#include <cmath>

#include "d3d.hpp"
#include "scene.hpp"
#include "debug.hpp"
#include "vertex.hpp"

namespace engine
{
	void Scene::init()
	{
		// shaders for main triangle pipeline
		std::vector<ShaderInfo> shaders1 = {
			{ShaderType::VERTEX, L"render/shaders/cube.hlsl", "VSMain"},
			{ShaderType::PIXEL,  L"render/shaders/cube.hlsl",  "PSMain"} 
		};

		std::vector<ShaderInfo> shaders2 = {
		{ShaderType::VERTEX, L"render/shaders/skybox.hlsl", "VSMain"},
		{ShaderType::PIXEL,  L"render/shaders/skybox.hlsl",  "PSMain"}
		};

		// create the input layout object
		std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		modelProgram.init(shaders1, ied);
		skyboxProgram.init(shaders2);
		

		modelProgram.createUniform(sizeof(XMMATRIX));
		skyboxProgram.createUniform(sizeof(XMVECTOR) * 4);
	}

	void Scene::addModel(Model model)
	{
		this->model = model;
	}

	void Scene::setSkybox(Skybox skybox)
	{
		this->skybox = skybox;
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

	void Scene::renderFrame(Window& window, Renderer& renderer, Camera& camera)
	{
		ALWAYS_ASSERT(window.m_renderTargetView != NULL && "render target is NULL");
		s_devcon->OMSetRenderTargets(1, window.m_renderTargetView.access(), window.m_pDSV); 

		FLOAT backgroundColor[4]{ 0.0f, 0.2f, 0.4f, 1.0f };
		s_devcon->ClearRenderTargetView(window.m_renderTargetView.ptr(), backgroundColor);
		s_devcon->ClearDepthStencilView(window.m_pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 0.0f, 0);

		renderer.render(modelProgram, window, camera, model);
		renderer.render(skyboxProgram, window, camera, skybox);

		DXGI_PRESENT_PARAMETERS pPresentParameters;
		pPresentParameters.DirtyRectsCount = 0;
		pPresentParameters.pDirtyRects = NULL;
		pPresentParameters.pScrollRect = NULL;
		pPresentParameters.pScrollOffset = NULL;
		window.m_swapchain->Present1(0, 0, &pPresentParameters);
	}

	void Scene::clean()
	{
		model.cleanBuffers();
		skybox.clean();

		modelProgram.clean();
		skyboxProgram.clean();
	}

	void Scene::findIntersectionInternal(const ray& r, ObjRef& outRef, Intersection& outNearest, Material*& outMaterial)
	{
	}

	void Scene::findIntersectionShadow(const ray& r, ObjRef& outRef, Intersection& outNearest, Material*& outMaterial)
	{
	}
}