#include "scene.h"


class Controller
{
	bool m_rmbDown;
	Scene scene;

public:
	Controller()
	{
		m_rmbDown = false;
		scene.addSphere(vec3(200.0f, 100.0f, -100.0f), 50);
	}

	MSG mainLoop(Window& window, MSG& msg);
	LRESULT CALLBACK processInput(HWND& hWnd, UINT& message, WPARAM& wParam, LPARAM& lParam);
};