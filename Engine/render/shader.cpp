#include "shader.h"

#include <d3dx11.h>

#pragma comment (lib, "d3dx11.lib")

void engine::ShaderProgram::init(std::vector<ShaderInfo>& shaders, std::vector<D3D11_INPUT_ELEMENT_DESC>& ied)
{
	m_ied = ied;

	for (ShaderInfo& shader : shaders)
	{
		switch (shader.type)
		{
		case ShaderType::VERTEX:
		{
			D3DX11CompileFromFile(shader.filePath, 0, 0, shader.funcName, "vs_4_0", 0, 0, 0, &VS, 0, 0);
			ALWAYS_ASSERT(VS != nullptr && "vertex shader compilation failed");
			// encapsulate both shaders into shader objects
			s_device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, m_pVS.access());
		}break;
		case ShaderType::PIXEL:
		{
			D3DX11CompileFromFile(shader.filePath, 0, 0, shader.funcName, "ps_4_0", 0, 0, 0, &PS, 0, 0);
			ALWAYS_ASSERT(PS != nullptr && "pixel shader compilation failed");
			s_device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, m_pPS.access());
		}break;
		default:
		{}
		}
	}
	
	// set the shader objects
	s_devcon->VSSetShader(m_pVS, 0, 0);
	s_devcon->PSSetShader(m_pPS, 0, 0);

	auto res = s_device->CreateInputLayout(m_ied.data(), 2, VS->GetBufferPointer(), VS->GetBufferSize(), m_pLayout.access());
	ALWAYS_ASSERT(res == S_OK);

	s_devcon->IASetInputLayout(m_pLayout);
}

