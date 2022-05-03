#pragma once

#include <windows.h>
#include <windowsx.h>

#include <iostream>
#include <vector>
#include <memory>

#include "lightSource.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class Window
{
public:
	int m_width;
	int m_height;
	bool m_rmbDown;
	HBITMAP m_pixelBuf;
	HWND hWnd;
	WNDCLASSEX wc;
	HDC hdc;

	Window() {}

	Window(int width, int height, _In_ HINSTANCE& hInstance, _In_opt_ HINSTANCE& hPrevInstance, LPSTR& lpCmdLine, int nCmdShow)
	{
		m_width = width;
		m_height = height;
		m_rmbDown = false;
		init(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	}

	void init(_In_ HINSTANCE& hInstance, _In_opt_ HINSTANCE& hPrevInstance, LPSTR& lpCmdLine, int nCmdShow);


	friend LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	friend class Renderer;
};