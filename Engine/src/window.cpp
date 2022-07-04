#include "window.h"
#include "debug.h"
#include "constants.h"
#include "vertex.h"

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

namespace engine
{
	void Window::init(_In_ HINSTANCE& hInstance, _In_opt_ HINSTANCE& hPrevInstance, LPSTR& lpCmdLine, int nCmdShow)
	{
		ZeroMemory(&wc, sizeof(WNDCLASSEX));

		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = hInstance;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.lpszClassName = L"WindowClass1";

		RegisterClassEx(&wc);

		RECT wr = { 0, 0, m_width, m_height };    // set the size, but not the position
		AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

		hWnd = CreateWindowEx(NULL,
			L"WindowClass1",    // name of the window class
			L"Engine",   // title of the window
			WS_OVERLAPPEDWINDOW,    // window style
			300,    // x-position of the window
			300,    // y-position of the window
			wr.right - wr.left,    // width of the window
			wr.bottom - wr.top,    // height of the window
			NULL,    // we have no parent window, NULL
			NULL,    // we aren't using menus, NULL
			hInstance,    // application handle
			NULL);    // used with multiple windows, NULL

		ShowWindow(hWnd, nCmdShow);

		hdc = GetDC(hWnd);
	}

	void Window::onResize(const int& width, const int& height)
	{
		m_width = width;
		m_height = height;
	}

	XMVECTOR Window::screenToNDC(const uint16_t& x, const uint16_t& y) const
	{
		float xNDC = (2.0f * x) / m_width - 1.0f;
		float yNDC = 1.0f - (2.0f * y) / m_height;
		return XMVectorSet(xNDC, yNDC, 1.0f, 1.0f);
	}

	void Window::initSwapchain()
	{
		DXGI_SWAP_CHAIN_DESC1 desc;

		// clear out the struct for use
		memset(&desc, 0, sizeof(DXGI_SWAP_CHAIN_DESC1));

		// fill the swap chain description struct
		desc.AlphaMode = DXGI_ALPHA_MODE::DXGI_ALPHA_MODE_UNSPECIFIED;
		desc.BufferCount = 2;
		desc.BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.Flags = 0;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;                               // how many multisamples
		desc.SampleDesc.Quality = 0;                             // ???
		desc.Scaling = DXGI_SCALING_NONE;
		desc.Stereo = false;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		HRESULT res = s_factory->CreateSwapChainForHwnd(s_device, hWnd, &desc, NULL, NULL, m_swapchain.access());

		ALWAYS_ASSERT(res >= 0 && "CreateSwapChainForHwnd");
	}

	void Window::initBackBuffer() // may be called after resizing
	{

		if (m_backbuffer)
		{
			m_backbuffer->Release();
			m_swapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		}

		HRESULT result = m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)m_backbuffer.access());
		ALWAYS_ASSERT(result >= 0);
		result = s_device->CreateRenderTargetView(m_backbuffer, NULL, m_renderTargetView.access());
		m_backbuffer->Release();
		ALWAYS_ASSERT(result >= 0);

		// set the render target as the back buffer

		ID3D11Texture2D* pTextureInterface = 0;
		m_backbuffer->QueryInterface<ID3D11Texture2D>(&pTextureInterface);
		pTextureInterface->GetDesc(&m_backbufferDesc);
		pTextureInterface->Release();

	}

	void Window::initViewPort()
	{
		ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
		m_viewport.TopLeftX = 0;
		m_viewport.TopLeftY = 0;
		m_viewport.Width = SCREEN_WIDTH;
		m_viewport.Height = SCREEN_HEIGHT;
		s_devcon->RSSetViewports(1, &m_viewport);
	}
}