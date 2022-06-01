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

	std::vector<uint32_t> m_pixels;

public:

	Canvas() = default;
	Canvas(int width, int height, int offsetX, int offsetY);
	
	BITMAPINFO createDIB();

	void setPixel(int x, int y, BYTE r, BYTE g, BYTE b); // set color into m_pixels
	void printToScreen(const HDC, const HWND, uint16_t windWidth, uint16_t windHeight); // call StretchDIBits / SetDIBitsToDevice

	void onResize(const int& width, const int& height);

	inline int getWidth() const { return m_width; }
	inline int getHeight() const { return m_height; }
};