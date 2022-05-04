#include "canvas.h"

#include <iostream>

Canvas::Canvas(int width, int height, int offsetX, int offsetY)
//	: m_pixels(width * height * 4, 1)
{
	m_width = width;
	m_height = height;
	//m_pixels = std::vector<BYTE>();

	bmi = createDIB();;


	//std::cout << "pixel: " <<  m_pixels[100] << std::endl;
}

BITMAPINFO Canvas::createDIB()
{
	int iBmiSize;
	int iSurfaceSize;


	// Initialize bitmap info header
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = m_width;
	bmi.bmiHeader.biHeight = -(signed)m_height; // To render image top-down
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biSizeImage = 0;
	bmi.bmiHeader.biXPelsPerMeter = 0;
	bmi.bmiHeader.biYPelsPerMeter = 0;
	bmi.bmiHeader.biClrUsed = 0;
	bmi.bmiHeader.biClrImportant = 0;
	bmi.bmiHeader.biCompression = BI_RGB;


	DWORD* pBmi = (DWORD*)bmi.bmiColors;

	pBmi[0] = 0x00FF0000;	// Red mask
	pBmi[1] = 0x0000FF00;	// Green mask
	pBmi[2] = 0x000000FF;	// Blue mask
	pBmi[3] = 0x00000000;	// Not used (Alpha?)

	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression |= BI_BITFIELDS;

	return bmi;
}

void Canvas::setPixel(int x, int y, BYTE r, BYTE g, BYTE b)
{
	int iOffset = bmi.bmiHeader.biWidth * y + x;

	m_pixels[iOffset*4] = b;
	m_pixels[iOffset*4 + 1] = g;
	m_pixels[iOffset*4 + 2] = r;
	m_pixels[iOffset*4 + 3] = 1;
}

void Canvas::printToScreen(const HDC hdc, const HWND hWnd)
{

	SetDIBitsToDevice(
		hdc,			// Target DC
		0,			// Destination X-coord.
		0,			// Destination Y-coord.
		m_width,		// DIB width in pixels
		m_height,		// DIB height in pixels
		0,			// Source X-coord.
		0,			// Source Y-coord.
		0,			// Starting scanline
		m_height,		// Number of scanlines
		(BYTE*)&m_pixels[0],		// Pointer to the DIB surface data
		&bmi,	// Pointer to the BITMAPINFO struct.
		DIB_RGB_COLORS	// Display mode
	);
	//InvalidateRect(hWnd, NULL, FALSE);
}
