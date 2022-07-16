#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include <DirectXMath.h>
#include "DxRes.hpp"
#include "debug.hpp"

using namespace DirectX;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
namespace engine
{
	class Window
	{
		D3D11_VIEWPORT m_viewport;
		D3D11_TEXTURE2D_DESC m_backbufferDesc;

		DxResPtr<ID3D11Texture2D> m_pDepthStencil;
		DxResPtr<ID3D11DepthStencilState> m_pDSState;

	public:
		DxResPtr<ID3D11RenderTargetView> m_renderTargetView;
		DxResPtr<IDXGISwapChain1> m_swapchain;             // the pointer to the swap chain interface
		DxResPtr<ID3D11Texture2D> m_backbuffer;    // the pointer to our back buffer
		DxResPtr<ID3D11DepthStencilView> m_pDSV;

		uint16_t m_width;
		uint16_t m_height;
		HWND hWnd = nullptr;
		WNDCLASSEX wc;
		HDC hdc;

		Window() = default;
		Window(int width, int height, _In_ HINSTANCE& hInstance, _In_opt_ HINSTANCE& hPrevInstance, LPSTR& lpCmdLine, int nCmdShow)
		{
			m_width = width;
			m_height = height;
			init(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
		}

		void init(_In_ HINSTANCE& hInstance, _In_opt_ HINSTANCE& hPrevInstance, LPSTR& lpCmdLine, int nCmdShow);
		void clean();
		void onResize(uint16_t width, uint16_t height);
		XMVECTOR screenToNDC(uint16_t x, uint16_t y) const;

		//DirectX

		void initSwapchain();
		void initBackBuffer();
		void initViewPort();

		void bindDepthStencil();


		friend LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		friend class Scene;
		friend class Renderer;

	private:
		void createDepthStencilBuffer();
	};
}