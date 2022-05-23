#include "application.h"

void Application::init(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow, int width, int height)
{
	//std::cout << vec << " + " << vec2 << " = " << vec + vec2 << std::endl;
	this->window = Window(width, height, hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	controller.init(this->window, scene);
}

void Application::initConsole()
{
	if (!AllocConsole()) {
		// Add some error handling here.
		// You can call GetLastError() to get more info about the error.
		return;
	}
	FILE* dummy;
	auto s = freopen_s(&dummy, "CONOUT$", "w", stdout);
}

MSG Application::run()
{
	MSG msg = { 0 };
	std::chrono::time_point<std::chrono::steady_clock> oldTime = std::chrono::high_resolution_clock::now();

	while (TRUE)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);

			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				break;
		}

		m_deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - oldTime).count();

		if (m_deltaTime > FRAME_DURATION)
		{
			oldTime = std::chrono::high_resolution_clock::now();
			controller.update(m_deltaTime, scene);
			controller.processFrame(window, scene);
			window.flush();

			std::cout << "fps: " << 1.0f / m_deltaTime << std::endl;
		}
	}
	return msg;

}
