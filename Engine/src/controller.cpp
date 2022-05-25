#include "controller.h"

#include <chrono>

std::vector<Triangle> cubeVertices{
			{{-10.0f, -10.0f, -10.0f}, // 1
			{ 10.0f, -10.0f, -10.0f},
			{ 10.0f,  10.0f, -10.0f}},
			{{ 10.0f,  10.0f, -10.0f}, // 2
			{-10.0f,  10.0f, -10.0f},
			{-10.0f, -10.0f, -10.0f}},
			{{-10.0f, -10.0f,  10.0f}, // 3
			{ 10.0f, -10.0f,  10.0f},
			{ 10.0f,  10.0f,  10.0f}},
			{{ 10.0f,  10.0f,  10.0f}, // 4
			{-10.0f,  10.0f,  10.0f},
			{-10.0f, -10.0f,  10.0f}},
			{{-10.0f,  10.0f,  10.0f}, // 5
			{-10.0f,  10.0f, -10.0f},
			{-10.0f, -10.0f, -10.0f}},
			{{-10.0f, -10.0f, -10.0f}, // 6
			{-10.0f, -10.0f,  10.0f},
			{-10.0f,  10.0f,  10.0f}},
			{{ 10.0f,  10.0f,  10.0f}, // 7
			{ 10.0f,  10.0f, -10.0f},
			{ 10.0f, -10.0f, -10.0f}},
			{{ 10.0f, -10.0f, -10.0f}, // 8
			{ 10.0f, -10.0f,  10.0f},
			{ 10.0f,  10.0f,  10.0f}},
			{{-10.0f, -10.0f, -10.0f}, // 9
			{ 10.0f, -10.0f, -10.0f},
			{ 10.0f, -10.0f,  10.0f}},
			{{ 10.0f, -10.0f,  10.0f}, // 10
			{-10.0f, -10.0f,  10.0f},
			{-10.0f, -10.0f, -10.0f}},
			{{-10.0f,  10.0f, -10.0f}, // 11
			{ 10.0f,  10.0f, -10.0f},
			{ 10.0f,  10.0f,  10.0f}},
			{{ 10.0f,  10.0f,  10.0f}, // 12
			{-10.0f,  10.0f,  10.0f},
			{-10.0f,  10.0f, -10.0f}},
};
std::shared_ptr<Mesh> cube = std::make_shared<Mesh>(cubeVertices);
std::shared_ptr<math::Sphere> sphere = std::make_shared <math:: Sphere> (XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), 20.0f);

void Controller::init(Window& win, Scene& scene)
{
	Material bronze(XMVectorSet(0.2125f, 0.1275f, 0.054f, 0.0f), XMVectorSet(0.714f, 0.4284f, 0.18144f, 0.0f),
					XMVectorSet(0.393548f, 0.271906f, 0.166721f, 0.0f), 0.2f);
	Material greenRubber(XMVectorSet(0.0f, 0.05f, 0.0f, 0.0f), XMVectorSet(0.4f, 0.5f, 0.4f, 0.0f),
		XMVectorSet(0.04f, 0.7f, 0.04f, 0.0f), 0.078125f);

	scene.setSurface(Surface(Plane(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, -100.0f, 0.0f, 0.0f)), greenRubber));

	scene.addModel(Model(XMVectorSet(40.0f, -20.0f, -100.0f, 0.0f), XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), Angles(0.0f, 0.0f, 0.0f), cube, bronze));

	scene.addSphere(SphereModel(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), 1.0f, Angles(0.0f, 0.0f, 0.0f), sphere, bronze));

	scene.addPointLight(PointLight(XMVectorSet(130.0f, -60.0f, -110.0f, 0.0f), XMVectorSet(0.3f, 0.3f, 0.3f, 0.0f),
		XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f), XMVectorSet(0.5f, 0.5f, 0.5f, 1.0f), XMVectorSet(0.9f, 0.9f, 0.9f, 1.0f)),
						SphereModel(XMVectorSet(130.0f, -60.0f, -110.0f, 0.0f), 1.0f, Angles(0.0f, 0.0f, 0.0f), sphere));

	//scene.addPointLight(PointLight(XMVectorSet(-130.0f, 1000.0f, -110.0f, 0.0f), XMVectorSet(0.2f, 0.2f, 0.9f, 0.0f),
		//XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f), XMVectorSet(0.5f, 0.5f, 0.5f, 1.0f), XMVectorSet(0.9f, 0.9f, 0.9f, 1.0f)),
		//SphereModel(XMVectorSet(-130.0f, 60.0f, -110.0f, 0.0f), 1.0f, Angles(0.0f, 0.0f, 0.0f), sphere));


	scene.addDirLight(DirectionalLight(XMVectorSet(-0.1f, -0.9f, 0.0f, 0.0f), XMVectorSet(0.3f, 0.3f, 0.3f, 0.0f),
		XMVectorSet(0.1f, 0.1f, 0.1f, 0.0f), XMVectorSet(0.3f, 0.3f, 0.3f, 0.0f), XMVectorSet(0.6f, 0.6f, 0.6f, 0.0f)));

	scene.addFlashLight(FlashLight(XMVectorSet(0.0f, -50.0f, 0.0f, 0.0f), XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f),
									 XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),	0.91f, 0.82f),
	SphereModel(XMVectorSet(0.0f, 50.0f, 0.0f, 0.0f), 1.0f, Angles(0.0f, 0.0f, 0.0f), sphere));

	m_camera = Camera(XMVectorSet(0.0f, 50.0f, 80.0f, 1.0f),  {0.0f, 0.0f, 0.0f});
	m_camera.setPerspective(45.0f, win.m_width, win.m_height, 0.1f, 800.0f);
	m_cameraSpeed = CAMERA_SPEED;
	m_mouseSensitivity = MOUSE_SENSITIVITY;

	m_rmbDown = false;
	m_lmbDown = false;
}

void Controller::update(float deltaTime, Scene& scene)
{
	m_camera.updateMatrices();
	scene.updateLightsPos();

	m_deltaTime = deltaTime;
	if (m_lmbDown)
	{
		rotateCamera(m_currentPos.x - m_pressedPos.x, m_currentPos.y - m_pressedPos.y, m_deltaTime);
	}
	if (m_rmbDown)
	{

	}
}

void Controller::processFrame(Window& window, Scene& scene)
{
	scene.render(window, m_camera.m_viewProjInv, m_camera);
}

LRESULT CALLBACK Controller::processInput(HWND& hWnd, UINT& message, WPARAM& wParam, LPARAM& lParam, Scene& scene, Window& window)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	} break;
	case WM_SIZE:
	{
		RECT rect = { 0, 0, 0, 0 };

		GetWindowRect(hWnd, &rect);

		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size
		window.onResize(rect.right - rect.left, rect.bottom - rect.top);
		m_camera.setPerspective(45.0f, window.m_width, window.m_height, 1.0f, 10000.0f);

	} break;
	case WM_MOUSEMOVE:
	{
		if (m_rmbDown)
		{
			POINT point;
			GetCursorPos(&point);
			//determine position relative to init window
			ScreenToClient(FindWindowA(NULL, "Engine"), &point);

			if (scene.objPicked)
			{
				GetCursorPos(&m_currentPos);
				ScreenToClient(FindWindowA(NULL, "Engine"), &m_currentPos);

				XMVECTOR point = window.screenToNDC(m_currentPos.x, m_currentPos.y);
				point = XMVector4Transform(point, m_camera.m_projInv);
				point /= XMVectorGetW(point);
				point = XMVector4Transform(point, m_camera.m_viewInv);

				ray r(m_camera.position(), point);
				r.direction = XMVector3Normalize(XMVectorSet(XMVectorGetX(r.direction), XMVectorGetY(r.direction), XMVectorGetZ(r.direction), 0.0f));


				r.direction = -r.origin + point;


				Intersection hr;
				hr.reset();
				scene.m_movingPlane.hit(r, hr);

				XMVECTOR offset = XMVectorSet(XMVectorGetX(hr.point) - XMVectorGetX(scene.pickedObjPtr->getPickedPos()),
												XMVectorGetY(hr.point) - XMVectorGetY(scene.pickedObjPtr->getPickedPos()), 
												XMVectorGetZ(hr.point) - XMVectorGetZ(scene.pickedObjPtr->getPickedPos()), 0.0f);
				//std::cout << XMVectorGetX(hr.point) << " " << XMVectorGetY(hr.point) << " " << XMVectorGetZ(hr.point) << std::endl;
				scene.pickedObjPtr->moveBy(offset);
				//scene.pickedObjPtr->moveTo(hr.point);
			}

		}
		else if (m_lmbDown)
		{
			GetCursorPos(&m_currentPos);
			ScreenToClient(FindWindowA(NULL, "Engine"), &m_currentPos);
		}
	} break;
	case WM_LBUTTONDOWN:
	{
		if (!m_lmbDown)
		{
			m_lmbDown = true;
			GetCursorPos(&m_pressedPos);
			//determine position relative to init window
			ScreenToClient(FindWindowA(NULL, "Engine"), &m_pressedPos);
			m_currentPos = m_pressedPos;
		}
	} break;
	case WM_LBUTTONUP:
	{
		m_lmbDown = false;
	} break;
	case WM_RBUTTONDOWN:
	{
		GetCursorPos(&m_pressedPos);
		ScreenToClient(FindWindowA(NULL, "Engine"), &m_pressedPos);
		m_rmbDown = true;
		m_currentPos = m_pressedPos;
		scene.pickObject(m_camera, window.screenToNDC(m_currentPos.x, m_currentPos.y));
	} break;
	case WM_RBUTTONUP:
	{
		m_rmbDown = false;
		scene.objPicked = false;
		scene.pickedObjPtr = nullptr;
	}
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case 'W':
		{
			moveCamera(XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f), m_deltaTime);
		} break;
		case 'A':
		{
			moveCamera(XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f), m_deltaTime);
		} break;
		case 'S':
		{
			moveCamera(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), m_deltaTime);
		} break;
		case 'D':
		{
			moveCamera(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), m_deltaTime);
		} break;
		case VK_CONTROL:
		{
			moveCamera(XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f), m_deltaTime);
		} break;
		case VK_SPACE:
		{
			moveCamera(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), m_deltaTime);
		}	
		break;
		case 'E' :
		{
			rotateCamera(-1.0f, m_deltaTime);
		}
		break;
		case 'Q':
		{
			rotateCamera(1.0f, m_deltaTime);
		}
		break;
		}
	} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void Controller::moveCamera(const XMVECTOR& direction, const float& deltaTime)
{
	XMVECTOR offset = direction * deltaTime * m_cameraSpeed;
	m_camera.addRelativeOffset(offset);
	m_camera.updateMatrices();
}

void Controller::rotateCamera(const float& xOffset, const float& yOffset, const float& deltaTime)
{
	Angles res;

	res.pitch =  -yOffset * deltaTime * m_mouseSensitivity;
	res.roll = 0.0f;
	res.yaw = -xOffset * deltaTime * m_mouseSensitivity;

	m_camera.addRelativeAngles(res);
}

void Controller::rotateCamera(const float& direction, const float& deltaTime)
{
	Angles res;

	res.pitch = 0.0f;
	float d = (direction > 0) ? 1.0f : -1.0f;
	res.roll = d * deltaTime * 10.0f;
	res.yaw = 0.0f;

	m_camera.addRelativeAngles(res);
}

