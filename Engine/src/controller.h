

#include "renderer.h"

class Controller
{
	bool m_rmbDown;

public:
	Controller() {}

	MSG mainLoop(Renderer& renderer, Window& window, MSG& msg);
	LRESULT CALLBACK processInput(HWND& hWnd, UINT& message, WPARAM& wParam, LPARAM& lParam, Renderer& renderer);
};