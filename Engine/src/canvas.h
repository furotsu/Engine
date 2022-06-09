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
	Canvas(uint32_t width, uint32_t height, uint32_t offsetX, uint32_t offsetY);
	
	BITMAPINFO createDIB();

	void setPixel(uint32_t x, uint32_t y, BYTE r, BYTE g, BYTE b); // set color into m_pixels
	void printToScreen(const HDC, const HWND, uint16_t windWidth, uint16_t windHeight); // call StretchDIBits / SetDIBitsToDevice

	void onResize(uint32_t width, uint32_t height);

	inline int getWidth() const { return m_width; }
	inline int getHeight() const { return m_height; }
};