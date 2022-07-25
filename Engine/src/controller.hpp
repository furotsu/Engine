#pragma once

#include <string>
#include <iostream>

#include "scene.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "camera.hpp"

namespace engine
{
	class Controller
	{
	public:
		bool m_rmbDown;
		bool m_lmbDown;
		bool userInputReceived;
		bool speedIncreased;

		bool m_mouseMoved;
		float m_cameraSpeed;
		float m_mouseSensitivity;

		float m_deltaTime;

		Camera m_camera;

		bool m_buttonsState[256];
		std::vector<uint16_t> m_activeButtons;

		POINT m_pressedPos;
		POINT m_currentPos;

		Controller() = default;

		void init(Window& win, Scene& scene);
		void clean();
		void processInput();

		void update(float deltaTime, Scene& scene, Window& window);

		void onKeyDown(uint16_t key);
		void onKeyUp(uint16_t key);
		void onResize(const Window& win);

		void processFrame(Window& window, Scene& scene);

		void changeCameraSpeed(float increase);
		void moveCamera(const XMVECTOR& direction);
		void rotateCamera(float xOffset, float yOffset);
		void rotateCamera(float direction);

	};
}
