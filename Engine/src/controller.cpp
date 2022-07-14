#include "controller.hpp"

#include <chrono>
#include <cmath>
#include "textureLoader.hpp"
#include "cube.hpp"

namespace engine
{
	void Controller::init(Window& win, Scene& scene)
	{
		Cube cube;
		std::shared_ptr<Mesh> cubeMesh = std::make_shared<Mesh>(cube.cubeVertices, cube.cubeIndices);
		scene.init();

		scene.addModel(Model(cubeMesh, L"container2.dds", 5.0f, XMVectorSet(10.0f, 0.0f, 30.0f, 0.0)));
		scene.setSkybox(Skybox(L"cubemap.dds"));
		renderer.init();

		m_camera = Camera(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), { 0.0f, 0.0f, 0.0f });
		m_camera.setPerspective(45.0f, win.m_width, win.m_height, 0.1f, 800.0f);
		m_cameraSpeed = CAMERA_SPEED;
		m_mouseSensitivity = MOUSE_SENSITIVITY;

		m_rmbDown = false;
		m_lmbDown = false;
		userInputReceived = true;

		//Create the buffer to send to the cbuffer in effect file
		D3D11_BUFFER_DESC cbbd;
		ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	}

	void Controller::clean()
	{
		renderer.clean();
	}

	void Controller::update(float deltaTime, Scene& scene, Window& window)
	{
		m_deltaTime = deltaTime;
		processInput();

		if (m_mouseMoved)
		{
			GetCursorPos(&m_currentPos);
			ScreenToClient(FindWindowA(NULL, "Engine"), &m_currentPos);
			if (m_lmbDown)
			{
				rotateCamera((m_currentPos.x - m_pressedPos.x) / (float)window.m_width, (m_currentPos.y - m_pressedPos.y) / (float)window.m_height);
				userInputReceived = true;
			}
		}
		m_camera.updateMatrices();
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

	void Controller::onResize(const Window& win)
	{
		m_camera.setPerspective(45.0f, win.m_width, win.m_height, NEAR_PLANE, FAR_PLANE); 
	}

	void Controller::processFrame(Window& window, Scene& scene)
	{
		PerFrameUniform uniform;
		uniform.g_viewProj = m_camera.m_viewProj;
		uniform.g_screenWidht = window.m_width;
		uniform.g_screenHeight = window.m_height;
		renderer.setPerFrameUniforms(uniform); 

		scene.renderFrame(window, renderer, m_camera);
	}

	void Controller::processInput()
	{
		//iterate through all possible buttons
		for (uint16_t& button : m_activeButtons)
		{
			switch (button)
			{
			case 'W':
			{
				moveCamera(XMVectorSet(0.0f, 0.0f, 0.1f, 0.0f)); 
			} break;
			case 'A':
			{
				moveCamera(XMVectorSet(-0.1f, 0.0f, 0.0f, 0.0f));
			} break;
			case 'S':
			{
				moveCamera(XMVectorSet(0.0f, 0.0f, -0.1f, 0.0f));
			} break;
			case 'D':
			{
				moveCamera(XMVectorSet(0.1f, 0.0f, 0.0f, 0.0f));
			} break;
			case 'Q':
			{
				moveCamera(XMVectorSet(0.0f, 0.1f, 0.0f, 0.0f));
			}break;
			case 'E':
			{
				moveCamera(XMVectorSet(0.0f, -0.1f, 0.0f, 0.0f));
			}break;
			case VK_SHIFT:
			{
				speedIncreased = true;
			}break;
			default:
			{
			}
			}
		}
	}

	void Controller::changeCameraSpeed(float increase)
	{
		float delta = math::lerp(1.0f, 1.1f, static_cast<float>(abs(increase)) / 120.0f);
		if (increase > 0)
			m_cameraSpeed *= (delta);
		else
			m_cameraSpeed /= (delta);
		m_cameraSpeed = math::clamp(m_cameraSpeed, MIN_CAMERA_SPEED, MAX_CAMERA_SPEED);
	}

	void Controller::moveCamera(const XMVECTOR& direction)
	{
		XMVECTOR offset = direction * m_deltaTime * m_cameraSpeed * ((speedIncreased) ? 5.0f : 1.0f);
		speedIncreased = false;
		m_camera.addRelativeOffset(offset);
		m_camera.updateMatrices();
	}

	void Controller::rotateCamera(float xOffset, float yOffset)
	{
		Angles res;

		res.pitch = yOffset * m_deltaTime * m_mouseSensitivity;
		res.roll = 0.0f;
		res.yaw = xOffset * m_deltaTime * m_mouseSensitivity;

		m_camera.addRelativeAngles(res);
	}

	void Controller::rotateCamera(float direction)
	{
		Angles res;

		res.pitch = 0.0f;
		float d = (direction > 0) ? -1.0f : 1.0f;
		res.roll = d * m_deltaTime * ROLL_ROTATION_SPEED;
		res.yaw = 0.0f;

		m_camera.addRelativeAngles(res);
	}
}