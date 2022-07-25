#include "window.hpp"
#include "debug.hpp"
#include "constants.hpp"
#include "vertex.hpp"

#include "d3d.hpp"


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
		m_width = wr.right - wr.left;
		m_height = wr.bottom - wr.top;
		hWnd = CreateWindowEx(NULL,
			L"WindowClass1",    // name of the window class
			L"Engine",   // title of the window
			WS_OVERLAPPEDWINDOW,    // window style
			300,    // x-position of the window
			300,    // y-position of the window
			m_width,    // width of the window
			m_height,    // height of the window
			NULL,    // we have no parent window, NULL
			NULL,    // we aren't using menus, NULL
			hInstance,    // application handle
			NULL);    // used with multiple windows, NULL

		ShowWindow(hWnd, nCmdShow);
		
		hdc = GetDC(hWnd);

		createDepthStencilBuffer();
		bindDepthStencil();

		initSwapchain();
		initBackBuffer();
		initViewPort();
		onResize(m_width, m_height); // to adjust height properly

	}

	void Window::clean()
	{
		m_pDepthStencil.release();
		m_pDSState.release();
		m_swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode
		m_swapchain.release();
		m_renderTargetView.release();
		m_backbuffer.release();
		m_pDSV.release();
	}

	void Window::onResize(uint16_t width, uint16_t height)
	{
		if (hWnd)
		{
			m_width = width;
			m_height = height;
			
			m_renderTargetView.release();
			m_backbuffer.release();
			m_swapchain->ResizeBuffers(NULL, m_width, m_height, DXGI_FORMAT_UNKNOWN, NULL);
			
			createDepthStencilBuffer();
			initBackBuffer();
			initViewPort();
		}
	}

	XMVECTOR Window::screenToNDC( uint16_t x,  uint16_t y) const
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
		desc.Flags = NULL;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;                               // how many multisamples
		desc.SampleDesc.Quality = 0;                            
		desc.Scaling = DXGI_SCALING_NONE;
		desc.Stereo = false;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		HRESULT res = s_factory->CreateSwapChainForHwnd(s_device, hWnd, &desc, NULL, NULL, m_swapchain.reset());

		ALWAYS_ASSERT(res >= 0 && "CreateSwapChainForHwnd");
	}

	void Window::initBackBuffer() // may be called after resizing
	{

		HRESULT result = m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)m_backbuffer.reset());
		ALWAYS_ASSERT(result >= 0);
		result = s_device->CreateRenderTargetView(m_backbuffer, NULL, m_renderTargetView.reset());
		ALWAYS_ASSERT(result >= 0);

		// set the render target as the back buffer

		ID3D11Texture2D* pTextureInterface = nullptr;
		m_backbuffer->QueryInterface<ID3D11Texture2D>(&pTextureInterface);
		pTextureInterface->GetDesc(&m_backbufferDesc);
		m_backbuffer.release();
		pTextureInterface->Release();
		pTextureInterface = nullptr;
	}

	void Window::initViewPort()
	{
		RECT rect = { 0, 0, 0, 0 };
		GetWindowRect(hWnd, &rect);

		ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
		m_viewport.TopLeftX = 0;
		m_viewport.TopLeftY = 0;
		m_viewport.Width = rect.right - rect.left;
		m_viewport.Height = rect.bottom - rect.top;
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		s_devcon->RSSetViewports(1, &m_viewport);
	}

	void Window::createDepthStencilBuffer()
	{
		//Create depth-stencil buffer using texture resource

		D3D11_TEXTURE2D_DESC descDepth;
		descDepth.Width = m_width;
		descDepth.Height = m_height;
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

	void Window::bindDepthStencil()
	{
		s_devcon->OMSetDepthStencilState(m_pDSState, 1);
	}
}