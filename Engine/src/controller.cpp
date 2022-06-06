#include "controller.h"

#include <chrono>
#include "cube.h"

Cube c;

std::shared_ptr<Mesh> cube = std::make_shared<Mesh>(c.cubeVertices);
std::shared_ptr<math::Sphere> sphere = std::make_shared <math:: Sphere> (XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), 20.0f);

void Controller::init(Window& win, Scene& scene)
{

	scene.EV100 = 2.0f;
	scene.reflectionsOn = true;
	scene.shadowsOn = false;

	Material materialSurface(XMVectorSet(0.4f, 0.9f, 0.4f, 0.0f), XMVectorSet(0.4f, 0.6f, 0.4f, 0.0f), 0.9f, 0.1f);
	Material materialModel(XMVectorSet(0.9f, 0.6f, 0.4f, 0.0f), XMVectorSet(0.9f, 0.6f, 0.4f, 0.0f), 0.6f, 0.1f);

	scene.addModel(Scene::Model(XMVectorSet(40.0f, -20.0f, -100.0f, 0.0f), XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), cube, materialModel));

	scene.setSurface(Scene::Surface(Plane(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, -100.0f, 0.0f, 0.0f)), materialSurface));

	for (uint16_t j = 0; j < 3; j++)
	{
		for (uint16_t i = 0; i < 12; i++)
		{
			Material mat = Material(XMVectorSet(0.3f + 0.1f * j, 0.2f, 0.3f + 0.05f * i, 0.0f), XMVectorSet(0.3f + 0.1f * j, 0.2f, 0.3f + 0.05f * i, 0.0f),\
									min(0.01f + 0.1f * i, 1.0f), 0.5f * j);
			scene.addSphere(Scene::Sphere(XMVectorSet(-150 + 30 * i, -15 + 30 * j, -200.0f, 0.0f), 10.0f, mat));
		}
	}

	scene.setAmbient({ 0.178f, 0.247f, 0.302f, 0.0f });

	scene.addPointLight(Scene::PointLight(XMVectorSet(-120.0f, 30.0f, -20.0f, 0.0f), XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), 25.0f));

	scene.addDirLight(Scene::DirectionalLight(XMVectorSet(-0.1f, -0.9f, 0.0f, 0.0f), XMVectorSet(0.3f, 0.3f, 0.3f, 0.0f), 0.5f));

	scene.addFlashLight(Scene::SpotLight(XMVectorSet(0.0f, -20.0f, 0.0f, 0.0f), XMVectorSet(5.0f, 5.0f, 5.0f, 0.0f), XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f),	0.91f, 0.32f, 8.0f));

	pickedObjMoverQuery = Scene::IntersectionQuery();
	pickedObjMoverQuery.intersection.reset();

	m_camera = Camera(XMVectorSet(0.0f, 20.0f, 300.0f, 1.0f),  {0.0f, 0.0f, 0.0f});
	m_camera.setPerspective(45.0f, win.m_width, win.m_height, 0.1f, 800.0f);
	m_cameraSpeed = CAMERA_SPEED;
	m_mouseSensitivity = MOUSE_SENSITIVITY;

	m_rmbDown = false;
	m_lmbDown = false;
	m_reflectionsOn = true;
}

void Controller::update(float deltaTime, Scene& scene, Window& window)
{
	m_deltaTime = deltaTime;
	processInput();

	scene.EV100 += EVchange;
	scene.reflectionsOn = m_reflectionsOn;
	EVchange = 0.0f;

	if (m_mouseMoved)
	{
		GetCursorPos(&m_currentPos);
		ScreenToClient(FindWindowA(NULL, "Engine"), &m_currentPos);
		if (m_lmbDown)
		{

			rotateCamera((m_currentPos.x - m_pressedPos.x)/(float)window.m_width, (m_currentPos.y - m_pressedPos.y)/(float)window.m_height);
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


				r.direction = XMVector3Normalize( - r.origin + point);
				
				XMVECTOR holdPoint = r.origin + r.direction * pickedObjMoverQuery.distToPickedObj;
			
				XMVECTOR offset = XMVectorSet(XMVectorGetX(holdPoint) - XMVectorGetX(pickedObjMoverQuery.mover->getPickedPos()),
					XMVectorGetY(holdPoint) - XMVectorGetY(pickedObjMoverQuery.mover->getPickedPos()),
					XMVectorGetZ(holdPoint) - XMVectorGetZ(pickedObjMoverQuery.mover->getPickedPos()), 0.0f);

				pickedObjMoverQuery.mover->moveBy(offset);
				m_mouseMoved = false;
			}
		}
	}

	m_camera.updateMatrices();
}

void Controller::onKeyDown(int key)
{
	m_buttonsState[key] = true;
	m_keyup = true;
}

void Controller::onKeyUp(int key)
{
	m_buttonsState[key] = false;
}

void Controller::processFrame(Window& window, Scene& scene, ParallelExecutor& executor)
{
	scene.render(window, m_camera, executor);
}

void Controller::processInput()
{
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
			case VK_CONTROL:
			{
				moveCamera(XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f));
			} break;
			case VK_SPACE:
			{
				moveCamera(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
			}break;
			case 'E':
			{
				rotateCamera(-1.0f);
			}break;
			case 'Q':
			{
				rotateCamera(1.0f);
			}break;
			case VK_OEM_PLUS:
			{
				EVchange += m_keyup ? 1.0f : 0.0f;
				m_keyup = false;

			}break;
			case VK_OEM_MINUS:
			{
				EVchange -= m_keyup ? 1.0f : 0.0f;
				m_keyup = false;
			}break;
			case 'R':
			{
				m_reflectionsOn = m_keyup ? !m_reflectionsOn : m_reflectionsOn;
				m_keyup = false;
			}
			default:
			{
			}
			}

		}
	}
	m_mouseMoved = true;
}

void Controller::moveCamera(const XMVECTOR& direction)
{
	XMVECTOR offset = direction * m_deltaTime * m_cameraSpeed;
	m_camera.addRelativeOffset(offset);
	m_camera.updateMatrices();
}

void Controller::rotateCamera(const float& xOffset, const float& yOffset)
{
	Angles res;

	res.pitch =  -yOffset * m_deltaTime * m_mouseSensitivity;
	res.roll = 0.0f;
	res.yaw = -xOffset * m_deltaTime * m_mouseSensitivity;

	m_camera.addRelativeAngles(res);
}

void Controller::rotateCamera(const float& direction)
{
	Angles res;

	res.pitch = 0.0f;
	float d = (direction > 0) ? 1.0f : -1.0f;
	res.roll = d * m_deltaTime * ROLL_ROTATION_SPEED;
	res.yaw = 0.0f;

	m_camera.addRelativeAngles(res);
}

