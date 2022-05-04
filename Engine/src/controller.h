#include <string>

#include "scene.h"

constexpr float FRAME_DURATION = 1.0f / 60.0f;

class Controller
{
	bool m_rmbDown;


public:
	Controller()
		: m_rmbDown(false)
	{
	}

	void init(Window& win, Scene& scene);

	MSG mainLoop(Window& window, MSG& msg, Scene& scene);
	LRESULT CALLBACK processInput(HWND& hWnd, UINT& message, WPARAM& wParam, LPARAM& lParam, Scene& scene, Window& w);
};