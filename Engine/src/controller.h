#include <string>

#include "scene.h"


class Controller
{
	bool m_rmbDown;
	Scene scene;

public:
	Controller()
		: m_rmbDown(false)
	{
	}

	void init(Window& win);

	MSG mainLoop(Window& window, MSG& msg);
	LRESULT CALLBACK processInput(HWND& hWnd, UINT& message, WPARAM& wParam, LPARAM& lParam);
};