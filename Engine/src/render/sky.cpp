#include "sky.hpp"

engine::Sky::Sky(std::string filepath)
{
	m_texture = TextureManager::getTexture(filepath, TextureType::TEXTURE_CUBEMAP);
}

void engine::Sky::init(std::vector<ShaderInfo>& shaders)
{
	m_skyboxProgram = ShaderManager::GetInstance()->getShader(shaders);

	if (m_skyboxProgram->isUniformsEmpty())
	{
		m_skyboxProgram->createUniform(sizeof(XMVECTOR) * 4);
	}
}

void engine::Sky::clean()
{
	m_texture = nullptr;
	m_skyboxProgram = nullptr;
}

void engine::Sky::render(const Window& window, const Camera& camera)
{
	bind();

	XMVECTOR TL = XMVectorSet(-1.0f, 3.0f, 1.0f, 1.0f);
	XMVECTOR BR = XMVectorSet(3.0f, -1.0f, 1.0f, 1.0f);
	XMVECTOR BL = XMVectorSet(-1.0f, -1.0f, 1.0f, 1.0f);

	TL = XMVector4Transform(TL, camera.getViewProjInv());
	BR = XMVector4Transform(BR, camera.getViewProjInv());
	BL = XMVector4Transform(BL, camera.getViewProjInv());

	TL /= XMVectorGetW(TL);
	BR /= XMVectorGetW(BR);
	BL /= XMVectorGetW(BL);

	XMVECTOR res[4]{ TL, BR, BL, camera.position() };
	std::vector<const void*> data;
	data.push_back(&res);
	m_skyboxProgram->bindUniforms(data);

	s_devcon->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	s_devcon->Draw(3, 0);
}

void engine::Sky::bind()
{
	m_skyboxProgram->bind();
	m_texture->bind();
}

