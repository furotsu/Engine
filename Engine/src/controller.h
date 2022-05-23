#pragma once

#include <string>
#include <iostream>

#include "scene.h"
#include "camera.h"

constexpr float FRAME_DURATION = 1.0f / 60.0f;
constexpr float CAMERA_SPEED = 50.0f;
constexpr float MOUSE_SENSITIVITY = 0.2f;


class Controller
{
	bool m_rmbDown;
	bool m_lmbDown;

	float m_cameraSpeed;
	float m_mouseSensitivity;

	float m_deltaTime;

	POINT m_pressedPos;
	POINT m_currentPos;
	
	Camera m_camera;

public:
	Controller() = default;

	void init(Window& win, Scene& scene);

	LRESULT CALLBACK processInput(HWND& hWnd, UINT& message, WPARAM& wParam, LPARAM& lParam, Scene& scene, Window& w);

	void update(float deltaTime, Scene& scene);
	void processFrame(Window& window, Scene& scene);


	void moveCamera(const XMVECTOR& direction,const float& deltTime);
	void rotateCamera(const float& xOffset, const float& yOffset, const float& deltaTime);
	void rotateCamera(const float& direction, const float& deltaTime);

};
