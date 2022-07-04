#pragma once

#include <string>
#include <iostream>

#include "scene.h"
#include "shader.h"

namespace engine
{
	class Controller
	{
	public:
		bool m_rmbDown;
		bool m_lmbDown;
		bool userInputReceived;

		float m_deltaTime;

		std::shared_ptr<Mesh> triangleMesh;
		Scene::Model triangle;
		ShaderProgram shaderProgram;

		bool m_buttonsState[256];
		std::vector<uint16_t> m_activeButtons;

		Controller() = default;

		void init(Window& win, Scene& scene);

		void processInput();

		void update(float deltaTime, Scene& scene, Window& window);

		void onKeyDown(uint16_t key);
		void onKeyUp(uint16_t key);

		void processFrame(Window& window, Scene& scene);

	};
}
