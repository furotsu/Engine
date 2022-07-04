#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include <DirectXMath.h>
#include "DxRes.h"
#include "debug.h"

using namespace DirectX;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
namespace engine
{
	class Window
	{
		D3D11_VIEWPORT m_viewport;
		D3D11_TEXTURE2D_DESC m_backbufferDesc;

		DxResPtr<ID3D11RenderTargetView> m_renderTargetView;
		DxResPtr<IDXGISwapChain1> m_swapchain;             // the pointer to the swap chain interface
		DxResPtr<ID3D11Texture2D> m_backbuffer;    // the pointer to our back buffer

	public:
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

		void onResize(const int& width, const int& height);
		XMVECTOR screenToNDC(const uint16_t& x, const uint16_t& y) const;

		//DirectX

		void initSwapchain();
		void initBackBuffer();
		void initViewPort();

		friend LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		friend class Scene;
	};
}