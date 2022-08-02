#include <math.h>
#include <algorithm>
#include <cmath>

#include "d3d.hpp"
#include "scene.hpp"
#include "debug.hpp"
#include "meshSystem.hpp"

namespace engine
{
	void Scene::init(const Window& window)
	{
		createDepthStencilBuffer(window.m_width, window.m_height);
		bindDepthStencil();
	}

	void Scene::addModel(std::shared_ptr<Model> model, std::vector<OpaqueInstances::Instance> positions)
	{
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
		s_devcon->OMSetRenderTargets(1, window.m_renderTargetView.access(), m_pDSV); 

		FLOAT backgroundColor[4]{ 0.0f, 0.2f, 0.4f, 1.0f };
		s_devcon->ClearRenderTargetView(window.m_renderTargetView.ptr(), backgroundColor);
		s_devcon->ClearDepthStencilView(m_pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 0.0f, 0);

		MeshSystem::GetInstance()->render();
		skybox.render(window, camera);
	}

	void Scene::onResize(const Window& window)
	{
		if(window.hWnd)
			createDepthStencilBuffer(window.m_width, window.m_height);
	}

	void Scene::bindDepthStencil()
	{
		s_devcon->OMSetDepthStencilState(m_pDSState, 1);
	}

	void Scene::clean()
	{
		skybox.clean();

		m_pDepthStencil.release();
		m_pDSState.release();
		m_pDSV.release();
	}

	void Scene::findIntersectionInternal(const ray& r, ObjRef& outRef, Intersection& outNearest, Material*& outMaterial)
	{
	}

	void Scene::findIntersectionShadow(const ray& r, ObjRef& outRef, Intersection& outNearest, Material*& outMaterial)
	{
	}


	void Scene::createDepthStencilBuffer(uint32_t width, uint32_t height)
	{
		//Create depth-stencil buffer using texture resource
		D3D11_TEXTURE2D_DESC descDepth;
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = NULL;
		descDepth.MiscFlags = NULL;

		HRESULT hr = s_device->CreateTexture2D(&descDepth, NULL, m_pDepthStencil.reset());
		ASSERT(hr >= 0 && " cannot create depth-stencil uniform buffer");

		//Create depth-stencil state
		D3D11_DEPTH_STENCIL_DESC dsDesc;

		// Depth test parameters
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;

		// Stencil test parameters
		dsDesc.StencilEnable = true;
		dsDesc.StencilReadMask = 0xFF;
		dsDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create depth stencil state
		ID3D11DepthStencilState* pDSState;
		s_device->CreateDepthStencilState(&dsDesc, m_pDSState.reset());

		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDSV.Flags = NULL;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		hr = s_device->CreateDepthStencilView(m_pDepthStencil, &descDSV, m_pDSV.reset());
		ASSERT(hr >= 0 && " cannot create depth-stencil view");

	}
}