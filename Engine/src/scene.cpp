#include <math.h>
#include <algorithm>
#include <cmath>

#include "d3d.hpp"
#include "scene.hpp"
#include "debug.hpp"
#include "vertex.hpp"

namespace engine
{
	bool Scene::findIntersection(const ray& r, IntersectionQuery& query)
	{
		ObjRef ref = { nullptr, IntersectedType::NUM };

		findIntersectionInternal(r, ref, query.intersection, query.materialPtr);
		switch (ref.type)
		{
		}
		return ref.type != IntersectedType::NUM;
	}

	void Scene::renderFrame(Window& window, Model& model)
	{

		ALWAYS_ASSERT(window.m_renderTargetView != NULL && "render target is NULL");
		s_devcon->OMSetRenderTargets(1, window.m_renderTargetView.access(), NULL);

		// clear the back buffer to a deep blue
		FLOAT backgroundColor[4] { 0.0f, 0.2f, 0.4f, 1.0f };
		s_devcon->ClearRenderTargetView(window.m_renderTargetView.ptr(), backgroundColor);

		// select which vertex buffer to display
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		s_devcon->IASetVertexBuffers(0, 1, model.m_pVBuffer.access(), &stride, &offset);

		// select which primtive type we are using
		s_devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw the vertex buffer to the back buffer
		// switch the back buffer and the front buffer
		s_devcon->Draw(3, 0);

		DXGI_PRESENT_PARAMETERS pPresentParameters;
		pPresentParameters.DirtyRectsCount = 0;
		pPresentParameters.pDirtyRects = NULL;
		pPresentParameters.pScrollRect = NULL;
		pPresentParameters.pScrollOffset = NULL;
		window.m_swapchain->Present1(0, 0, &pPresentParameters);
	}

	void Scene::findIntersectionInternal(const ray& r, ObjRef& outRef, Intersection& outNearest, Material*& outMaterial)
	{
	}

	void Scene::findIntersectionShadow(const ray& r, ObjRef& outRef, Intersection& outNearest, Material*& outMaterial)
	{
	}

}