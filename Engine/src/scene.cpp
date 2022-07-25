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
	}

	void Scene::addModel(Model model, std::vector<ShaderInfo> shaders, std::vector<D3D11_INPUT_ELEMENT_DESC> ied)
	{
		this->model = model;
		this->modelProgram = ShaderManager::GetInstance()->getShader(shaders, ied);

		if (modelProgram->isUniformsEmpty())
		{
			modelProgram->createUniform(sizeof(XMMATRIX));
		}

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

		renderCube(window, camera);
		skybox.render(window, camera);

		window.m_swapchain->Present(0, 0);
	}

	void Scene::renderCube(Window& window, const Camera& camera)
	{
		modelProgram->bind();
		model.bindTexture();

		std::vector<const void*> data;
		XMMATRIX modelMat = model.getModelMat();

		data.push_back(&modelMat);
		modelProgram->bindUniforms(data);

		// select which vertex buffer to display
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		s_devcon->IASetVertexBuffers(0, 1, model.m_pVBuffer.access(), &stride, &offset);

		s_devcon->IASetIndexBuffer(model.m_pIBuffer, DXGI_FORMAT_R32_UINT, 0);

		// select which primtive type we are using
		s_devcon->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		s_devcon->DrawIndexed(model.m_mesh->m_indices.size(), 0, 0);
	}

	void Scene::clean()
	{
		model.cleanBuffers();
		skybox.clean();

		modelProgram = nullptr;
	}

	void Scene::findIntersectionInternal(const ray& r, ObjRef& outRef, Intersection& outNearest, Material*& outMaterial)
	{
	}

	void Scene::findIntersectionShadow(const ray& r, ObjRef& outRef, Intersection& outNearest, Material*& outMaterial)
	{
	}
}