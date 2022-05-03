#include "canvas.h"



Canvas::Canvas(Window& parent, int width, int height, int offsetX, int offsetY)
{
	m_width = width;
	m_height = height;
	createDIB(width, height, m_pixels);
}

LPBITMAPINFO Canvas::createDIB(int x, int y, BYTE*& pBits)
{
	int iBmiSize;
	int iSurfaceSize;

	iBmiSize = sizeof(BITMAPINFO) + sizeof(DWORD) * 4;
	iSurfaceSize = x * y * sizeof(DWORD);

	if ((lpBmi = (LPBITMAPINFO)malloc(iBmiSize)) == NULL) 
	{
		return NULL;
	}

	ZeroMemory(lpBmi, iBmiSize);

	// Allocate memory for the DIB surface.
	if ((pBits = (BYTE*)malloc(iSurfaceSize)) == NULL)
	{
		return NULL;
	}

	ZeroMemory(pBits, iSurfaceSize);

	// Initialize bitmap info header
	lpBmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpBmi->bmiHeader.biWidth = x;
	lpBmi->bmiHeader.biHeight = -(signed)y; // To render image top-down
	lpBmi->bmiHeader.biPlanes = 1;
	lpBmi->bmiHeader.biSizeImage = 0;
	lpBmi->bmiHeader.biXPelsPerMeter = 0;
	lpBmi->bmiHeader.biYPelsPerMeter = 0;
	lpBmi->bmiHeader.biClrUsed = 0;
	lpBmi->bmiHeader.biClrImportant = 0;
	lpBmi->bmiHeader.biCompression = BI_RGB;


	DWORD* pBmi = (DWORD*)lpBmi->bmiColors;

	pBmi[0] = 0x00FF0000;	// Red mask
	pBmi[1] = 0x0000FF00;	// Green mask
	pBmi[2] = 0x000000FF;	// Blue mask
	pBmi[3] = 0x00000000;	// Not used (Alpha?)

	lpBmi->bmiHeader.biBitCount = 32;
	lpBmi->bmiHeader.biCompression |= BI_BITFIELDS;
}

void Canvas::setPixel(int x, int y, BYTE r, BYTE g, BYTE b)
{
	int iOffset = lpBmi->bmiHeader.biWidth * y + x;
	// Cast void* to a DWORD* and write pixel to surface
	DWORD* p = (DWORD*)m_pixels;
	p[iOffset] = (DWORD)((r << 16) | (g << 8) | b);

	/*
	uint32_t offset = y * m_width + x;

#ifdef _DEBUG
	if (offset >= m_width * m_height)
	{
		std::cout << "trying to set pixel off the screen" << std::endl;
	}
#endif

	m_pixels[offset] = (uint32_t)((r << 16) | (g << 8) | b);
	*/
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
		(BYTE*)m_pixels,		// Pointer to the DIB surface data
		lpBmi,		// Pointer to the BITMAPINFO struct.
		DIB_RGB_COLORS	// Display mode
	);
	InvalidateRect(hWnd, NULL, FALSE);
}

Canvas::~Canvas()
{
	/*
	if (m_pixels)
	{
		free(m_pixels);
	}

	if (lpBmi)
	{
		free(lpBmi);
	}
	*/
}