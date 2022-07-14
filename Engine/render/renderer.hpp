#pragma once

#include "d3d.h"
#include "DXRes.hpp"

#include "model.hpp"
#include "skybox.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "window.hpp"

namespace engine
{
	struct PerFrameUniform
	{
		DirectX::XMMATRIX g_viewProj;
		FLOAT g_screenWidht;
		FLOAT g_screenHeight;
		XMFLOAT2 padding0;
	};

	class Renderer
	{
		DxResPtr<ID3D11Buffer> g_uniformGlobal;

	public:
		Renderer() = default;
		void init();
		void clean();

		void setPerFrameUniforms(const PerFrameUniform& data);

		void render(ShaderProgram& program, Window& window, Camera& camera, Model& model);
		void render(ShaderProgram& program, Window& window, Camera& camera, Skybox& skybox);

	};
}