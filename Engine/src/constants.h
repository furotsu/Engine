#pragma once
#include <DirectXMath.h>

using namespace DirectX;

//main
//Initial values for window size
constexpr int SCREEN_WIDTH = 400;
constexpr int SCREEN_HEIGHT = 200;

//Controller
constexpr float FRAME_DURATION = 1.0f / 60.0f;
constexpr float CAMERA_SPEED = 100.0f;
constexpr float ROLL_ROTATION_SPEED = 60.0f;
constexpr float MIN_CAMERA_SPEED = 0.1f;
constexpr float MAX_CAMERA_SPEED = 1000.0f;

// half of screen - 180 degrees/second 
constexpr float MOUSE_SENSITIVITY = 180.0f;

//Scene
constexpr float LIGHTMODEL_SIZE = 5.0f;
constexpr uint32_t MAX_REFLECTION_DEPTH = 2u;
constexpr float MAX_REFLECTION_ROUGHNESS = 0.12f;
constexpr float REFLECTION_ROUGNESS_MULTIPLIER = 1.0f / MAX_REFLECTION_ROUGHNESS;
constexpr uint32_t RAYS_ABOVE_HEMISPHERE_COUNT = 100u;

constexpr float LIGHT_POWER_MULTIPLIER = 10.0f;

//ray minimum and maximum length
constexpr float RAY_MIN = 1.0f;
constexpr float RAY_MAX = std::numeric_limits<float>::infinity();