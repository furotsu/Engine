#pragma once

#include <windows.h>
#include <windowsx.h>

#include <iostream>
#include <vector>
#include <memory>

#include <DirectXMath.h>
#include "canvas.h"

using namespace DirectX;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class Window
{
public:
	uint16_t m_width;
	uint16_t m_height;
	HBITMAP m_pixelBuf;
	HWND hWnd;
	WNDCLASSEX wc;
	HDC hdc;

	Canvas canvas;
	
	Window()= default;

	Window(int width, int height, _In_ HINSTANCE& hInstance, _In_opt_ HINSTANCE& hPrevInstance, LPSTR& lpCmdLine, int nCmdShow)
		:canvas(width, height, 0, 0)
	{
		m_width = width;
		m_height = height;
		init(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	}

	void init(_In_ HINSTANCE& hInstance, _In_opt_ HINSTANCE& hPrevInstance, LPSTR& lpCmdLine, int nCmdShow);

	void flush();

	void onResize(const int& width, const int& height);
	XMVECTOR screenToNDC(const uint16_t& x, const uint16_t& y) const;

	friend LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	friend class Scene;
};