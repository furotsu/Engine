#include "canvas.h"

#include <iostream>

Canvas::Canvas(int width, int height, int offsetX, int offsetY)
{
	m_width = width - width % 4;
	m_height = height;
	m_pixels = std::vector<uint32_t>(width * height );

	bmi = createDIB();;
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

	bmi.bmiHeader.biBitCount = 32;

	return bmi;
}

void Canvas::onResize(const int& width, const int& height)
{
	m_pixels.clear();

	m_width = width - width % 4;
	m_height = height;

	bmi.bmiHeader.biWidth = m_width;
	bmi.bmiHeader.biHeight = -(signed)m_height;

	m_pixels = std::vector<uint32_t>(m_width * m_height );
}

void Canvas::setPixel(int x, int y, BYTE r, BYTE g, BYTE b)
{
	int iOffset = bmi.bmiHeader.biWidth * y + x;

	m_pixels[iOffset] = (r << 16) + (g << 8) + (b);
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

}
