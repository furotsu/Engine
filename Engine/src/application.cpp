#include "application.hpp"
#include "globals.hpp"
#include "textureManager.hpp"

namespace engine
{
	void Application::init(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow, int width, int height)
	{
		this->window = Window(width, height, hInstance, hPrevInstance, lpCmdLine, nCmdShow);

		//init globals
		Globals::init();
		TextureManager::init();
			
		window.initSwapchain();
		window.initBackBuffer();
		window.initViewPort();

		controller.init(window, scene);
	}

	void Application::clean()
	{
		scene.clean();
		controller.clean();
		window.clean();
		TextureManager::clean();
		Globals::clean();
	}

	void Application::initConsole()
	{
		if (!AllocConsole()) {
			return;
		}
		FILE* dummy;
		auto s = freopen_s(&dummy, "CONOUT$", "w", stdout);
	}

	LRESULT Application::processInput(HWND& hWnd, UINT& message, WPARAM& wParam, LPARAM& lParam, Scene& scene, Window& w)
	{
		switch (message)
		{
		case WM_DESTROY:
		{
			// close the application entirely
			clean();
			PostQuitMessage(0);
			return 0;
		} break;
		case WM_SIZE:
		{
			RECT rect = { 0, 0, 0, 0 };
			GetWindowRect(hWnd, &rect);

			window.onResize(rect.right - rect.left, rect.bottom - rect.top);
			controller.onResize(window);
			controller.userInputReceived = true;
		} break;
		case WM_MOUSEMOVE:
		{
			controller.m_mouseMoved = true;
		} break;
		case WM_LBUTTONDOWN:
		{
			controller.m_lmbDown = true;
			GetCursorPos(&controller.m_pressedPos);
			//determine position relative to init window
			ScreenToClient(FindWindowA(NULL, "Engine"), &controller.m_pressedPos);
			controller.m_currentPos = controller.m_pressedPos;
			controller.userInputReceived = true;
		} break;
		case WM_LBUTTONUP:
		{
			controller.m_lmbDown = false;
		} break;
		case WM_KEYDOWN:
		{
			controller.onKeyDown(wParam);
			controller.userInputReceived = true;
		} break;
		case WM_KEYUP:
		{
			controller.onKeyUp(wParam);
		}break;
		case WM_MOUSEWHEEL:
		{
			controller.changeCameraSpeed(GET_WHEEL_DELTA_WPARAM(wParam));
		}break;
		default:
		{
		}
		}

		// Handle any messages the switch statement didn't
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	MSG Application::run(ParallelExecutor& executor)
	{
		MSG msg = { 0 };

		timer.recordTime();

		while (TRUE)
		{
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);

				DispatchMessage(&msg);
				if (msg.message == WM_QUIT)
					return msg;
			}

			m_deltaTime = timer.elapsed();
			if (m_deltaTime > FRAME_DURATION)
			{
				controller.update(m_deltaTime, scene, window);
				controller.processFrame(window, scene);
				timer.recordTime();

				std::cout << "fps: " << 1.0f / m_deltaTime << std::endl;
			}
		}

		return msg;

	}
}