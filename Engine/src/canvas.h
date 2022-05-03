#include <windows.h>
#include <windowsx.h>

#include <memory>
#include <vector>
#include <array>

#include "window.h"

class Canvas
{
	int m_width;
	int m_height;
	BITMAPINFO m_bmi;
	LPBITMAPINFO lpBmi;

	//std::unique_ptr<uint32_t> m_pixels;
	BYTE* m_pixels;

public:
	Canvas() {};

	~Canvas();
	Canvas(Window& parent, int width, int height, int offsetX, int offsetY);
	
	LPBITMAPINFO createDIB(int x, int y, BYTE*& pBits);

	void setPixel(int x, int y, BYTE r, BYTE g, BYTE b); // set color into m_pixels
	void printToScreen(const HDC, const HWND); // call StretchDIBits / SetDIBitsToDevice

};