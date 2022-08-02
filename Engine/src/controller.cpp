#include "controller.hpp"

#include <chrono>
#include <cmath>
#include "textureLoader.hpp"
#include "globals.hpp"
#include "opaque_instances.hpp"
#include "meshSystem.hpp"

namespace engine
{
	void Controller::init(Window& win, Scene& scene)
	{

		// shaders for main triangle pipeline
		std::vector<ShaderInfo> shaders1 = {
			{ShaderType::VERTEX, L"shaders/modelInstanced.hlsl", "VSMain"},
			{ShaderType::PIXEL,  L"shaders/modelInstanced.hlsl",  "PSMain"}
		};

		std::vector<ShaderInfo> shaders2 = {
		{ShaderType::VERTEX, L"shaders/skybox.hlsl", "VSMain"},
		{ShaderType::PIXEL,  L"shaders/skybox.hlsl",  "PSMain"}
		};

		// create the input layout object for cube model
		std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"ROWX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"ROWY", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"ROWZ", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"ROWW", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1}
		};

		scene.init(win);
		MeshSystem::GetInstance()->initOpaqueInstances(shaders1, ied);


		std::vector<OpaqueInstances::Instance> wallPositions;
		std::vector<OpaqueInstances::Instance> horsePositions;
		std::vector<OpaqueInstances::Instance> knightPositions;
		std::vector<OpaqueInstances::Instance> samuraiPositions;

		for (uint32_t i = 0; i != 20; ++i)
		{
			wallPositions.push_back(  XMMatrixTranslation( -20.0f + 3.15f * i, 5.0f, 0.0f ));
			horsePositions.push_back( XMMatrixTranslation( -20.0f + 1.575f * i, 0.0f, 0.9f ));
			horsePositions.push_back( XMMatrixTranslation( -20.0f + 1.575f * i * 2.0f, 0.0f, 0.9f ));
			knightPositions.push_back(XMMatrixTranslation( -20.0f + 1.575f * i, 0.0f, 0.0f ));
			knightPositions.push_back(XMMatrixTranslation( -20.0f + 1.575f * i * 2.0f, 0.0f, 0.0f ));
			samuraiPositions.push_back(XMMatrixTranslation(-20.0f + 3.15f * i, 4.8f, -5.2f));
		}

		MeshSystem::GetInstance()->addOpaqueInstances(ModelManager::GetInstance()->getModel("assets/models/Knight/", "Knight.fbx"), knightPositions);
		MeshSystem::GetInstance()->addOpaqueInstances(ModelManager::GetInstance()->getModel("assets/models/KnightHorse/", "KnightHorse.fbx"), horsePositions);
		MeshSystem::GetInstance()->addOpaqueInstances(ModelManager::GetInstance()->getModel("assets/models/SunCityWall/", "SunCityWall.fbx"), wallPositions);
		MeshSystem::GetInstance()->addOpaqueInstances(ModelManager::GetInstance()->getModel("assets/models/Samurai/", "Samurai.fbx"), samuraiPositions);
		scene.setSkybox(Sky("assets/cubemap.dds"), shaders2);

		m_camera = Camera(XMVectorSet(0.0f, 10.0f, -100.0f, 1.0f), { 0.0f, 0.0f, 0.0f });
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
		Globals::GetInstance()->bind(window, m_camera);
		scene.bindDepthStencil();
		scene.renderFrame(window, m_camera);
		window.m_swapchain->Present(0, 0);
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
				moveCamera(m_camera.forward()); 
			} break;
			case 'A':
			{
				moveCamera(-m_camera.right());
			} break;
			case 'S':
			{
				moveCamera(-m_camera.forward());
			} break;
			case 'D':
			{
				moveCamera(m_camera.right());
			} break;
			case 'Q':
			{
				moveCamera(XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f));
			}break;
			case 'E':
			{
				moveCamera(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
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
		m_camera.addWorldOffset(offset);
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