#include "shader.hpp"

#include "d3d.hpp"

void engine::ShaderProgram::init(std::vector<ShaderInfo>& shaders, std::vector<D3D11_INPUT_ELEMENT_DESC>& ied)
{
	m_ied = ied;

	for (ShaderInfo& shader : shaders)
	{
		switch (shader.type)
		{
		case ShaderType::VERTEX:
		{
			compileShader(shader, VS);
			s_device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, m_pVS.access());
		}break;
		case ShaderType::PIXEL:
		{

			compileShader(shader, PS);
			s_device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, m_pPS.access());
		}break;
		default:
		{}
		}
	}

	// set the shader objects
	s_devcon->VSSetShader(m_pVS, NULL, NULL);
	s_devcon->PSSetShader(m_pPS, NULL, NULL);

	auto res = s_device->CreateInputLayout(m_ied.data(), 2, VS->GetBufferPointer(), VS->GetBufferSize(), m_pLayout.access());
	ALWAYS_ASSERT(res == S_OK);

	s_devcon->IASetInputLayout(m_pLayout);
}

void engine::ShaderProgram::release()
{
	m_pLayout.release();
	m_pVS.release();
	m_pPS.release();
}

void engine::ShaderProgram::compileShader(const ShaderInfo& shader, ID3D10Blob*& blob)
{
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif
	LPCSTR profile = (shader.type == ShaderType::PIXEL) ? "ps_5_0" : "vs_5_0";
	ID3D10Blob* errorBlob = nullptr;
	HRESULT result = D3DCompileFromFile(shader.filePath, NULL, NULL, shader.funcName, profile, flags, NULL, &blob, &errorBlob);

	if (result)
	{
		if (errorBlob)
		{
			std::cout << (char*)errorBlob->GetBufferPointer();
		}
		else
		{
			std::wstring wsrt(shader.filePath);
			std::cout << std::string(std::string(wsrt.begin(), wsrt.end())) + " compilation failed";
		}
	}
}

void engine::ShaderProgram::bind()
{
	s_devcon->VSSetShader(m_pVS, NULL, NULL);
	s_devcon->PSSetShader(m_pPS, NULL, NULL);
}

void engine::ShaderProgram::unbind()
{
	s_devcon->VSSetShader(NULL, NULL, NULL);
	s_devcon->PSSetShader(NULL, NULL, NULL);
}