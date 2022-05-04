#pragma once

#include <windows.h>
#include <windowsx.h>

#include <iostream>
#include <vector>
#include <memory>

#include "lightSource.h"
#include "canvas.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class Window
{
	Window();
public:
	int m_width;
	int m_height;
	HBITMAP m_pixelBuf;
	HWND hWnd;
	WNDCLASSEX wc;
	HDC hdc;

	Canvas canvas;
	

	Window(int width, int height, _In_ HINSTANCE& hInstance, _In_opt_ HINSTANCE& hPrevInstance, LPSTR& lpCmdLine, int nCmdShow)
		:canvas(width, height, 0, 0)
	{
		m_width = width;
		m_height = height;
		init(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	}

	void init(_In_ HINSTANCE& hInstance, _In_opt_ HINSTANCE& hPrevInstance, LPSTR& lpCmdLine, int nCmdShow);

	friend LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	friend class Scene;
};