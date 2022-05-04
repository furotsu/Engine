#include <windows.h>
#include <windowsx.h>

#include <memory>
#include <vector>
#include <array>


class Canvas
{
	int m_width;
	int m_height;
	BITMAPINFO bmi;

	std::vector<BYTE> m_pixels;

	Canvas();
public:

	Canvas(int width, int height, int offsetX, int offsetY);
	
	BITMAPINFO createDIB();

	void setPixel(int x, int y, BYTE r, BYTE g, BYTE b); // set color into m_pixels
	void printToScreen(const HDC, const HWND); // call StretchDIBits / SetDIBitsToDevice

};