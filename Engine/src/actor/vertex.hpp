#pragma once
#include "d3d.hpp"
#include <DirectXMath.h>
using namespace DirectX;

namespace engine
{
	struct Vertex
	{
		XMFLOAT3 Position;
		XMFLOAT4 Color;
	};
}