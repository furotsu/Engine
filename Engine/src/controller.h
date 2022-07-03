#pragma once

#include <string>
#include <iostream>

#include "scene.h"
#include "camera.h"

class Controller
{
public:
	bool m_rmbDown;
	bool m_lmbDown;
	bool userInputReceived;
	bool sceneDrawn;
	bool speedIncreased;

	float EVvalue;
	bool m_mouseMoved;
	float m_cameraSpeed;
	float m_mouseSensitivity;

	bool m_reflectionsOn;
	bool m_globalIlluminationOn;

	Scene::IntersectionQuery pickedObjMoverQuery;

	float m_deltaTime;

	bool m_buttonsState[256];
	std::vector<uint16_t> m_activeButtons;

	POINT m_pressedPos;
	POINT m_currentPos;
	
	Camera m_camera;

	Controller() = default;

	void init(Window& win, Scene& scene);

	void processInput();

	void update(float deltaTime, Scene& scene, Window& window);

	void onKeyDown(uint16_t key);
	void onKeyUp(uint16_t key);

	void processFrame(Window& window, Scene& scene, ParallelExecutor& executor);

	void changeEv(float valuePerSec);

	void changeCameraSpeed(float increase);
	void moveCamera(const XMVECTOR& direction);
	void rotateCamera(float xOffset, float yOffset);
	void rotateCamera(float direction);

};
