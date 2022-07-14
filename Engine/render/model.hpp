#pragma once

#include "DxRes.hpp"
#include "mesh.hpp"
#include "transform.hpp"
#include "texture.hpp"
#include "textureManager.hpp"

namespace engine
{
	class Model
	{
		DxResPtr<ID3D11Buffer> m_pVBuffer;
		DxResPtr<ID3D11Buffer> m_pIBuffer;
		std::shared_ptr<Mesh> m_mesh;
		Transform m_transformation;
		std::shared_ptr<Texture> m_texture;
	
	public:
	
		Model() = default;
		Model(const std::shared_ptr<Mesh>& mesh, const wchar_t* textureFilepath, float scale = 1.0f, XMVECTOR position = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
	
		void initBuffers();
		void cleanBuffers();
		void bindTexture();
		XMMATRIX getModelMat();
		friend class Scene;
		friend class Renderer;
	};
}