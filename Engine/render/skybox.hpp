#pragma once

#include "d3d.h"
#include "textureManager.hpp"

namespace engine
{
	class Skybox
	{
		std::shared_ptr<Texture> m_texture;


	public:
		Skybox() = default;
		Skybox(const wchar_t* filepath);

		void bind();
		void clean();
	};
}