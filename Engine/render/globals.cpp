#include "win.hpp"
#include "globals.hpp"
#include "debug.hpp"
// Say NVidia or AMD driver to prefer a dedicated GPU instead of an integrated.
// This has effect on laptops.
extern "C"
{
    _declspec(dllexport) uint32_t NvOptimusEnablement = 1;
    _declspec(dllexport) uint32_t AmdPowerXpressRequestHighPerformance = 1;
}

namespace engine
{
	Globals::Globals()
	{
		initD3D();
		initGlobalUniforms();
		initSharedSampleState();
	}

	void Globals::init()
	{
		if (s_globals == nullptr)
		{
			s_globals = new Globals();
		}
		else
		{
			ERROR("Initializing \" Globals \" singleton more than once ");
		}
	}

	void Globals::deinit()
	{
		if (s_globals == nullptr)
		{
			ERROR("Trying to delete \" Globals \" singleton more than once ");
		}
		else
		{
			clean();
			delete s_globals;
			s_globals = nullptr;
		}
	}

	Globals* Globals::GetInstance()
	{
		if (s_globals == nullptr)
		{
			ERROR("Trying to call \" Globals \" singleton  instance before initializing it");
		}
		return s_globals;
	}

	void Globals::initD3D()
	{
		HRESULT result;

		result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)m_factory.reset());
		ASSERT(result >= 0 && "CreateDXGIFactory");

		result = m_factory->QueryInterface(__uuidof(IDXGIFactory5), (void**)m_factory5.reset());
		ALWAYS_ASSERT(result >= 0 && "Query IDXGIFactory5");

		{
			uint32_t index = 0;
			IDXGIAdapter1* adapter;
			while (m_factory5->EnumAdapters1(index++, &adapter) != DXGI_ERROR_NOT_FOUND)
			{
				DXGI_ADAPTER_DESC1 desc;
				adapter->GetDesc1(&desc);
				std::wstring wsrt(desc.Description);
				LOG("Globals.cpp", "GPU # " + std::to_string(index) + " " + std::string(wsrt.begin(), wsrt.end()));
			}
		}

		// Init D3D Device & Context
		const D3D_FEATURE_LEVEL featureLevelRequested = D3D_FEATURE_LEVEL_11_0;
		D3D_FEATURE_LEVEL featureLevelInitialized = D3D_FEATURE_LEVEL_11_0;
		result = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG,
			&featureLevelRequested, 1, D3D11_SDK_VERSION, m_device.reset(), &featureLevelInitialized, m_devcon.reset());
		ALWAYS_ASSERT(result >= 0 && "D3D11CreateDevice");
		ALWAYS_ASSERT(featureLevelRequested == featureLevelInitialized && "D3D_FEATURE_LEVEL_11_0");

		result = m_device->QueryInterface(__uuidof(ID3D11Device5), (void**)m_device5.reset());
		ALWAYS_ASSERT(result >= 0 && "Query ID3D11Device5");

		result = m_devcon->QueryInterface(__uuidof(ID3D11DeviceContext4), (void**)m_devcon4.reset());
		ALWAYS_ASSERT(result >= 0 && "Query ID3D11DeviceContext4");

		result = m_device->QueryInterface(__uuidof(ID3D11Debug), (void**)m_devdebug.reset());
		ALWAYS_ASSERT(result >= 0 && "Query ID3D11Debug");

		// Write global pointers

		s_factory = m_factory5.ptr();
		s_device = m_device5.ptr();
		s_devcon = m_devcon4.ptr();
	}

	void Globals::initGlobalUniforms()
	{
		//Create global uniform buffer
		D3D11_BUFFER_DESC cbbd;
		ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

		cbbd.Usage = D3D11_USAGE_DYNAMIC;
		cbbd.ByteWidth = sizeof(PerFrameUniform);
		cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbbd.MiscFlags = 0;

		HRESULT hr = s_device->CreateBuffer(&cbbd, NULL, m_uniformGlobal.reset());
		ASSERT(hr >= 0 && " cannot create global uniform buffer");
	}

	void Globals::initSharedSampleState()
	{
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		HRESULT hr = s_device->CreateSamplerState(&sampDesc, m_pSharedSampleState.reset());
		ASSERT(hr >= 0 && L"unable to create shared sampler state");
	}

	void Globals::clean()
	{
		s_globals->m_factory.release();
		s_globals->m_factory5.release();
		s_globals->m_device.release();
		s_globals->m_device5.release();
		s_globals->m_devcon.release();
		s_globals->m_devdebug.release();
		s_globals->m_devcon4.release();

		s_globals->m_uniformGlobal.release();
		s_globals->m_pSharedSampleState.release();
	}

	void Globals::bind(const Window& window, const Camera& camera)
	{
		uniform.g_viewProj = camera.getViewProj();
		uniform.g_screenWidth = window.m_width;
		uniform.g_screenHeight = window.m_height;
		setPerFrameUniforms(uniform);
		bindSharedSampleState();
	}

	void Globals::setPerFrameUniforms(PerFrameUniform& data)
	{
		D3D11_MAPPED_SUBRESOURCE res;

		s_devcon->Map(m_uniformGlobal, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &res);
		memcpy(res.pData, &data, sizeof(PerFrameUniform));
		s_devcon->Unmap(m_uniformGlobal, NULL);

		s_devcon->VSSetConstantBuffers(0, 1, m_uniformGlobal.access());
		s_devcon->PSSetConstantBuffers(0, 1, m_uniformGlobal.access());
	}

	void Globals::bindSharedSampleState()
	{

		s_devcon->PSSetSamplers(0, 1, m_pSharedSampleState.access());
	}
}