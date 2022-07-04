#include "controller.h"

#include <chrono>
#include <cmath>

#include "cube.h"
namespace engine
{
	void Controller::init(Window& win, Scene& scene)
	{
		std::vector<Vertex> triangleVertices =
		{
			{0.0f, 0.7f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
			{0.45f, -0.5, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) },
			{-0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f) }
		};

		triangleMesh = std::make_shared<Mesh>(triangleVertices);

		triangle.init(triangleMesh);

		// shaders for main triangle pipeline
		std::vector<ShaderInfo> shaders1 = {
			{ShaderType::VERTEX, L"render/shaders/vertex.shader", "main"},
			{ShaderType::PIXEL,  L"render/shaders/pixel.shader",  "main"} };

		// create the input layout object
		std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		shaderProgram.init(shaders1, ied);
		
		m_rmbDown = false;
		m_lmbDown = false;
		userInputReceived = true;
	}

	void Controller::update(float deltaTime, Scene& scene, Window& window)
	{
		m_deltaTime = deltaTime;
		processInput();
	}

	void Controller::onKeyDown(uint16_t key)
	{
		userInputReceived = true;
		switch (key)
		{
		default:
		{
			if (!m_buttonsState[key])
			{
				m_activeButtons.push_back(key);
				m_buttonsState[key] = true;
			}
		}
		}
	}

	void Controller::onKeyUp(uint16_t key)
	{
		m_activeButtons.erase(std::remove(m_activeButtons.begin(), m_activeButtons.end(), key), m_activeButtons.end());
		m_buttonsState[key] = false;
	}

	void Controller::processFrame(Window& window, Scene& scene)
	{
		scene.renderFrame(window, triangle);
	}

	void Controller::processInput()
	{
		//iterate through all possible buttons
		for (uint16_t& button : m_activeButtons)
		{
			switch (button)
			{
			default:
			{
			}
			}
		}
	}
}