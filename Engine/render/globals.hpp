#pragma once

#include "DxRes.hpp"

namespace engine
{
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
	public:
		Globals();
		Globals(Globals& other) = delete;
		void operator=(const Globals&) = delete;

		static void init();
		static Globals* GetInstance();

		void initD3D();
		static void clean();
	};
}