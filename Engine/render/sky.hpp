#pragma once

#include "d3d.h"
#include "DXRes.hpp"
#include "shader.hpp"
#include "textureManager.hpp"
#include "camera.hpp"
#include "window.hpp"
#include "shaderManager.hpp"


namespace engine
{
	class Sky
	{
		std::shared_ptr<ShaderProgram> m_skyboxProgram;
		std::shared_ptr<Texture> m_texture;

	public:

		Sky() = default;
		Sky& operator=(const Sky& other)
		{
			m_texture = other.m_texture;
			return *this;
		}

		Sky(const wchar_t* filepath);

		void init(std::vector<ShaderInfo>& shaders);
		void clean();

		void render(const Window& window, const Camera& camera);

		void bind();
	};
}