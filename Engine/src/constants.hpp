#pragma once
#include <DirectXMath.h>


#include <limits>
using namespace DirectX;

//main
//Initial values for window size
constexpr int SCREEN_WIDTH = 400;
constexpr int SCREEN_HEIGHT = 200;
constexpr float NEAR_PLANE = 1.0f;
constexpr float FAR_PLANE = 1000.0f;

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
constexpr uint32_t MAX_REFLECTION_DEPTH = 4u;
constexpr float MAX_REFLECTION_ROUGHNESS = 0.12f;
constexpr float REFLECTION_ROUGNESS_MULTIPLIER = 1.0f / MAX_REFLECTION_ROUGHNESS;
constexpr uint32_t RAYS_ABOVE_HEMISPHERE_COUNT = 2000u;

constexpr float LIGHT_POWER_MULTIPLIER = 10.0f;

//ray minimum and maximum length
constexpr float RAY_MIN = 0.0001f;
constexpr float RAY_MAX = std::numeric_limits<float>::infinity();

//utility
constexpr float FLOAT_COMPARE_EPSILON = 0.00000001f;
constexpr float GOLDEN_RATIO = 1.618033988749895; // same as : (1.0f + sqrtf(5.0f)) / 2.0f;

//small offset to avoid visual bugs
constexpr float SMALL_OFFSET = 0.0001f;