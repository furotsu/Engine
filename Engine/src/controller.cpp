#include "controller.h"

#include <chrono>
#include "cube.h"

Cube c;

std::shared_ptr<Mesh> cube = std::make_shared<Mesh>(c.cubeVertices);
std::shared_ptr<math::Sphere> sphere = std::make_shared <math:: Sphere> (XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), 20.0f);

void Controller::init(Window& win, Scene& scene)
{
	Material bronze(XMVectorSet(0.714f, 0.4284f, 0.18144f, 0.0f), 0.25f, 0.2f);
	Material greenRubber(XMVectorSet(0.4f, 0.5f, 0.4f, 0.0f), 0.1f, 0.078125f);

	scene.setSurface(Scene::Surface(Plane(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, -100.0f, 0.0f, 0.0f)), greenRubber));

	scene.addModel(Scene::Model(XMVectorSet(40.0f, -20.0f, -100.0f, 0.0f), XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), cube, bronze));
	scene.addSphere(Scene::Sphere(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), 10.0f, bronze));

	scene.setAmbient({ 0.2f, 0.2f, 0.2f, 0.0f });

	scene.addPointLight(Scene::PointLight(XMVectorSet(130.0f, 60.0f, -60.0f, 0.0f), XMVectorSet(0.9f, 0.3f, 0.3f, 0.0f)));

	scene.addDirLight(Scene::DirectionalLight(XMVectorSet(-0.1f, -0.9f, 0.0f, 0.0f), XMVectorSet(0.3f, 0.3f, 0.3f, 0.0f)));

	scene.addFlashLight(Scene::SpotLight(XMVectorSet(0.0f, -50.0f, 0.0f, 0.0f), XMVectorSet(5.0f, 5.0f, 5.0f, 0.0f), XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f),	0.91f, 0.82f));

	m_camera = Camera(XMVectorSet(0.0f, 20.0f, 300.0f, 1.0f),  {0.0f, 0.0f, 0.0f});
	m_camera.setPerspective(45.0f, win.m_width, win.m_height, 0.1f, 800.0f);
	m_cameraSpeed = CAMERA_SPEED;
	m_mouseSensitivity = MOUSE_SENSITIVITY;

	m_rmbDown = false;
	m_lmbDown = false;
}

void Controller::update(float deltaTime, WPARAM& wParam, Scene& scene, Window& window)
{
	m_deltaTime = deltaTime;

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
			if (scene.pickedObjMoverQuery.mover)
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
				
				XMVECTOR holdPoint = r.origin + r.direction * scene.distToPickedObj;
			
				XMVECTOR offset = XMVectorSet(XMVectorGetX(holdPoint) - XMVectorGetX(scene.pickedObjMoverQuery.mover->getPickedPos()),
					XMVectorGetY(holdPoint) - XMVectorGetY(scene.pickedObjMoverQuery.mover->getPickedPos()),
					XMVectorGetZ(holdPoint) - XMVectorGetZ(scene.pickedObjMoverQuery.mover->getPickedPos()), 0.0f);

				scene.pickedObjMoverQuery.mover->moveBy(offset);
				m_mouseMoved = false;
			}
		}
	}

	processInput(wParam);
	m_camera.updateMatrices();
}

void Controller::processFrame(Window& window, Scene& scene)
{
	scene.render(window, m_camera);
}

void Controller::processInput(WPARAM& wParam)
{
	switch (wParam)
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
		}
		break;
		case 'E':
		{
			rotateCamera(-1.0f);
		}
		break;
		case 'Q':
		{
			rotateCamera(1.0f);
		}
		break;
		default:
		{
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
	res.roll = d * m_deltaTime * 10.0f;
	res.yaw = 0.0f;

	m_camera.addRelativeAngles(res);
}

