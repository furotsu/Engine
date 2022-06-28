#include "controller.h"

#include <chrono>
#include <cmath>

#include "cube.h"
Cube c;

std::shared_ptr<Mesh> cube = std::make_shared<Mesh>(c.cubeVertices);
std::shared_ptr<math::Sphere> sphere = std::make_shared <math:: Sphere> (XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), 20.0f);

void Controller::init(Window& win, Scene& scene)
{

	scene.EV100 = 2.0f;
	scene.globalIlluminationOn = true;
	scene.reflectionsOn = true;
	scene.shadowsOn = true;

	scene.setAmbient({ 0.478f, 0.647f, 0.902f, 0.0f });
	scene.setAmbient({ 0.0f, 0.0f, 0.0f, 0.0f });

	scene.addPointLight(Scene::PointLight(XMVectorSet(-20.0f, 30.0f, -120.0f, 0.0f), XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), 20.0f, 5.0f));
	scene.addPointLight(Scene::PointLight(XMVectorSet(-120.0f, 30.0f, -20.0f, 0.0f), XMVectorSet(0.9f, 0.1f, 0.1f, 0.0f), 20.0f, 20.0f));

	scene.addDirLight(Scene::DirectionalLight(XMVectorSet(-0.1f, -0.9f, 0.0f, 0.0f), XMVectorSet(0.3f, 0.3f, 0.3f, 0.0f), 0.3f));

	scene.addFlashLight(Scene::SpotLight(XMVectorSet(0.0f, 20.0f, -12.0f, 0.0f), XMVectorSet(5.0f, 5.0f, 5.0f, 0.0f), XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f),	0.91f, 0.32f, 20.0f));

	Material materialSurface(XMVectorSet(0.99f, 0.99f, 0.99f, 0.0f), 0.9f, 0.1f);
	Material materialModel1(XMVectorSet(0.99f, 0.2f, 0.2f, 0.0f), 0.2f, 0.1f);
	Material materialModel2(XMVectorSet(0.99f, 0.99f, 0.99f, 0.0f), 0.8f, 0.1f);
	Material mat1 = Material(XMVectorSet(0.3f + 0.1f * 2, 0.2f, 0.3f + 0.05f * 7, 0.0f), \
		min(0.01f + 0.1f * 7, 1.0f), 0.1f);
	Material mat2 = Material(XMVectorSet(0.01f, 0.99f, 0.01f, 0.9f), \
		min(0.01f + 0.1f * 7, 1.0f), 0.1f);
	scene.addModel(Scene::Model(XMVectorSet(10.0f, -10.0f,  -45.0f, 0.0f), XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), cube, materialModel1));
	scene.addModel(Scene::Model(XMVectorSet(-10.0f, -25.0f, -30.0f, 0.0f), XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), cube, materialModel2));
	
	scene.addSphere(Scene::Sphere(XMVectorSet(10.0f, -25.0f, -20.0f, 0.0f), 10.0f, mat1));
	scene.addSphere(Scene::Sphere(XMVectorSet(-9.0f, -20.0f, -3.0f, 0.0f), 10.0f, mat2));
	scene.setSurface(Scene::Surface(Plane(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, -20.0f, 0.0f, 0.0f)), materialSurface));

	for (uint16_t j = 0; j < 3; j++)
	{
		for (uint16_t i = 0; i < 7; i++)
		{
			Material mat = Material(XMVectorSet(0.6f, 0.4f, 0.2f, 0.0f), \
									min(0.01f + 0.15f * i, 1.0f), 0.5f * j);
			scene.addSphere(Scene::Sphere(XMVectorSet(-250 + 30 * i, -15 + 30 * j, -200.0f, 0.0f), 10.0f, mat));
		}
	}
	
	pickedObjMoverQuery = Scene::IntersectionQuery();
	pickedObjMoverQuery.intersection.reset();

	m_camera = Camera(XMVectorSet(40.0f, 0.0f, 30.0f, 1.0f),  {0.0f, -13.0f, 40.0f});
	m_camera.setPerspective(45.0f, win.m_width, win.m_height, 0.1f, 800.0f);
	m_cameraSpeed = CAMERA_SPEED;
	m_mouseSensitivity = MOUSE_SENSITIVITY;
	
	m_rmbDown = false;
	m_lmbDown = false;
	m_reflectionsOn = true;
	m_globalIlluminationOn = false;
	userInputReceived = true;
	sceneDrawn = false;
	EVvalue = 2.0f;
}

void Controller::update(float deltaTime, Scene& scene, Window& window)
{
	m_deltaTime = deltaTime;
	processInput();
	if (userInputReceived)
	{
		scene.globalIlluminationOn = (scene.globalIlluminationOn) ? false : m_globalIlluminationOn;

		scene.EV100 = EVvalue;
		scene.reflectionsOn = m_reflectionsOn;
		sceneDrawn = false;
	}

	if (m_mouseMoved)
	{
		GetCursorPos(&m_currentPos);
		ScreenToClient(FindWindowA(NULL, "Engine"), &m_currentPos);
		if (m_lmbDown)
		{
			rotateCamera((m_currentPos.x - m_pressedPos.x) / (float)window.m_width, (m_currentPos.y - m_pressedPos.y) / (float)window.m_height);
			userInputReceived = true;
		}
		if (m_rmbDown)
		{
			if (pickedObjMoverQuery.mover)
			{
				GetCursorPos(&m_currentPos);
				ScreenToClient(FindWindowA(NULL, "Engine"), &m_currentPos);

				XMVECTOR point = window.screenToNDC(m_currentPos.x, m_currentPos.y);
				point = XMVector4Transform(point, m_camera.m_projInv);
				point /= XMVectorGetW(point);
				point = XMVector4Transform(point, m_camera.m_viewInv);

				ray r(m_camera.position(), point);
				r.direction = XMVector3Normalize(XMVectorSet(XMVectorGetX(r.direction), XMVectorGetY(r.direction), XMVectorGetZ(r.direction), 0.0f));


				r.direction = XMVector3Normalize(-r.origin + point);

				XMVECTOR holdPoint = r.origin + r.direction * pickedObjMoverQuery.distToPickedObj;

				XMVECTOR offset = XMVectorSet(XMVectorGetX(holdPoint) - XMVectorGetX(pickedObjMoverQuery.mover->getPickedPos()),
					XMVectorGetY(holdPoint) - XMVectorGetY(pickedObjMoverQuery.mover->getPickedPos()),
					XMVectorGetZ(holdPoint) - XMVectorGetZ(pickedObjMoverQuery.mover->getPickedPos()), 0.0f);

				pickedObjMoverQuery.mover->moveBy(offset);
				m_mouseMoved = false;
				userInputReceived = true;
			}
		}
	}
	m_camera.updateMatrices();
}

void Controller::onKeyDown(uint16_t key)
{
	m_buttonsState[key] = true;
	m_keydown = true;
}

void Controller::onKeyUp(uint16_t key)
{
	m_buttonsState[key] = false;
}

void Controller::processFrame(Window& window, Scene& scene, ParallelExecutor& executor)
{
	if (!scene.globalIlluminationOn)
	{
		scene.render(window, m_camera, executor);
		userInputReceived = false;
	}
	else if (!sceneDrawn)
	{
		scene.render(window, m_camera, executor);
		userInputReceived = false;
		sceneDrawn = true;
	}
}

void Controller::processInput()
{
	m_globalIlluminationOn = false;
	//iterate through all possible buttons
	for (int i = 0; i != 256; i++)
	{
		if (m_buttonsState[i])
		{
			switch (i)
			{
			case 'W':
			{
				moveCamera(XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f));
			} break;
			case 'A':
			{
				moveCamera(XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f));
			} break;
			case 'S':
			{
				moveCamera(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
			} break;
			case 'D':
			{
				moveCamera(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f));
			} break;
			case 'E':
			{
				moveCamera(XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f));
			}break;
			case 'Q':
			{
				moveCamera(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
			}break;
			case VK_OEM_PLUS:
			{
				changeEv(1.0f);
			}break;
			case VK_OEM_MINUS:
			{
				changeEv(-1.0f);
			}break;
			case 'R':
			{
				m_reflectionsOn = m_keydown ? !m_reflectionsOn : m_reflectionsOn;
				m_keydown = false;
			}break;
			case 'G':
			{
				m_globalIlluminationOn = m_keydown ? !m_globalIlluminationOn : m_globalIlluminationOn;;
				m_keydown = false;
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
	m_mouseMoved = true;
}

void Controller::changeEv(float valuePerSec)
{
	EVvalue += m_deltaTime * valuePerSec;
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

	res.pitch =  -yOffset * m_deltaTime * m_mouseSensitivity;
	res.roll = 0.0f;
	res.yaw = -xOffset * m_deltaTime * m_mouseSensitivity;

	m_camera.addRelativeAngles(res);
}

void Controller::rotateCamera(float direction)
{
	Angles res;

	res.pitch = 0.0f;
	float d = (direction > 0) ? 1.0f : -1.0f;
	res.roll = d * m_deltaTime * ROLL_ROTATION_SPEED;
	res.yaw = 0.0f;

	m_camera.addRelativeAngles(res);
}

