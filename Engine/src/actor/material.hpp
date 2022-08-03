#pragma once

#include "textureManager.hpp"

namespace engine
{
	struct Material
	{
		std::shared_ptr<Texture> texture;

		void clean() { texture = nullptr; }
	};
}