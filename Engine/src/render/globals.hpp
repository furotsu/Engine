#pragma once

#include "DirectXMath.h"
#include "DxRes.hpp"
#include "d3d.h"
#include "camera.hpp"
#include "window.hpp"

namespace engine
{
	struct PerFrameUniform
	{
		DirectX::XMMATRIX g_viewProj;
		FLOAT g_screenWidth;
		FLOAT g_screenHeight;
		DirectX::XMFLOAT2 padding0;
	};

	class Globals // a singletone for accessing global rendering resources
	{
	protected:
		static Globals* s_globals;

		DxResPtr<IDXGIFactory>		     m_factory;
		DxResPtr<IDXGIFactory5>			 m_factory5;
		DxResPtr<ID3D11Device>			 m_device;
		DxResPtr<ID3D11Device5>			 m_device5;
		DxResPtr<ID3D11DeviceContext>	 m_devcon;
		DxResPtr<ID3D11DeviceContext4>   m_devcon4;
		DxResPtr<ID3D11Debug>			 m_devdebug;

		DxResPtr<ID3D11Buffer> m_uniformGlobal;
		PerFrameUniform uniform;
		DxResPtr<ID3D11SamplerState> m_pSharedSampleState;


		void initD3D();
		void initGlobalUniforms();
		void initSharedSampleState();
		void setPerFrameUniforms(PerFrameUniform& data);
		void bindSharedSampleState();
	public:
		Globals();
		Globals(Globals& other) = delete;
		void operator=(const Globals&) = delete;

		static Globals* GetInstance();

		static void init();
		static void deinit();
		static void clean();

		void bind(const Window& window, const Camera& camera);
	};
}