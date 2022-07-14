#include "renderer.hpp"
#include "debug.hpp"

using namespace DirectX;

void engine::Renderer::init()
{
	//Create global uniform buffer
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(PerFrameUniform);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;
	
	HRESULT hr = s_device->CreateBuffer(&cbbd, NULL, g_uniformGlobal.reset());
	ASSERT(hr >= 0 && "cannot create global uniform buffer");
}

void engine::Renderer::clean()
{
	g_uniformGlobal.release();
}

void engine::Renderer::setPerFrameUniforms(const PerFrameUniform& data)
{
	s_devcon->UpdateSubresource(g_uniformGlobal, 0, NULL, &data, 0, 0);
	s_devcon->VSSetConstantBuffers(0, 1, g_uniformGlobal.access());
	s_devcon->PSSetConstantBuffers(0, 1, g_uniformGlobal.access());
}

void engine::Renderer::render(ShaderProgram& program, Window& window, Camera& camera, Model& model)
{
	program.bind();
	model.bindTexture();

	std::vector<const void*> data;
	XMMATRIX modelMat = model.getModelMat();

	data.push_back(&modelMat);
	program.bindUniforms(data);

	// select which vertex buffer to display
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	s_devcon->IASetVertexBuffers(0, 1, model.m_pVBuffer.access(), &stride, &offset);

	s_devcon->IASetIndexBuffer(model.m_pIBuffer, DXGI_FORMAT_R32_UINT, 0);

	// select which primtive type we are using
	s_devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	s_devcon->DrawIndexed(model.m_mesh->m_indices.size(), 0, 0);
}


void engine::Renderer::render(ShaderProgram& program, Window& window, Camera& camera, Skybox& skybox)
{
	program.bind();
	skybox.bind();

	XMVECTOR TL = XMVectorSet(-1.0f, 3.0f, 1.0f, 1.0f);
	XMVECTOR BR = XMVectorSet(3.0f, -1.0f, 1.0f, 1.0f);
	XMVECTOR BL = XMVectorSet(-1.0f, -1.0f, 1.0f, 1.0f);

	TL = XMVector4Transform(TL, camera.m_projInv);
	BR = XMVector4Transform(BR, camera.m_projInv);
	BL = XMVector4Transform(BL, camera.m_projInv);

	TL /= XMVectorGetW(TL);
	BR /= XMVectorGetW(BR);
	BL /= XMVectorGetW(BL);

	TL = XMVector4Transform(TL, camera.m_viewInv);
	BR = XMVector4Transform(BR, camera.m_viewInv);
	BL = XMVector4Transform(BL, camera.m_viewInv);

	XMVECTOR res[4]{ TL, BR, BL, camera.position()};
	std::vector<const void*> data;
	data.push_back(&res);
	program.bindUniforms(data);

	s_devcon->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	s_devcon->Draw(3, 0);
}
