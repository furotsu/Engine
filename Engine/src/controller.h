#pragma once

#include <string>
#include <iostream>

#include "scene.h"
#include "camera.h"

constexpr float FRAME_DURATION = 1.0f / 60.0f;
constexpr float CAMERA_SPEED = 150.0f;
constexpr float ROLL_ROTATION_SPEED = 60.0f;

// half of screen - 180 degrees/second 
constexpr float MOUSE_SENSITIVITY = 180.0f;


class Controller
{
public:
	bool m_rmbDown;
	bool m_lmbDown;
	bool m_keyup;

	float EVchange;
	bool m_mouseMoved;
	float m_cameraSpeed;
	float m_mouseSensitivity;
	bool m_reflectionsOn;
	Scene::IntersectionQuery pickedObjMoverQuery;

	float m_deltaTime;

	bool m_buttonsState[256];

	POINT m_pressedPos;
	POINT m_currentPos;
	
	Camera m_camera;

	Controller() = default;

	void init(Window& win, Scene& scene);

	void processInput();

	void update(float deltaTime, Scene& scene, Window& window);

	void onKeyDown(int key);
	void onKeyUp(int key);

	void processFrame(Window& window, Scene& scene, ParallelExecutor& executor);


	void moveCamera(const XMVECTOR& direction);
	void rotateCamera(const float& xOffset, const float& yOffset);
	void rotateCamera(const float& direction);

};
