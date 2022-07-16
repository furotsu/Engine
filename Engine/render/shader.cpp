#include "shader.hpp"

#include "d3d.hpp"

void engine::ShaderProgram::init(std::vector<ShaderInfo>& shaders)
{
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
}

void engine::ShaderProgram::init(std::vector<ShaderInfo>& shaders, std::vector<D3D11_INPUT_ELEMENT_DESC>& ied)
{
	m_ied = ied;
	init(shaders);
	
	auto res = s_device->CreateInputLayout(m_ied.data(), 2, VS->GetBufferPointer(), VS->GetBufferSize(), m_pLayout.access());
	ALWAYS_ASSERT(res == S_OK && "failed to create input layout for vertex shader" );

	s_devcon->IASetInputLayout(m_pLayout);
}

void engine::ShaderProgram::clean()
{
	for (auto& buffer : uniformBuffers)
		buffer.release();
	m_pVS.release();
	m_pPS.release();
	m_pLayout.release();
}

void engine::ShaderProgram::createUniform(UINT size)
{
	DxResPtr<ID3D11Buffer> pBuffer;

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = size;
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	s_device->CreateBuffer(&cbbd, NULL, pBuffer.reset());
	uniformBuffers.push_back(pBuffer);
}

void engine::ShaderProgram::bindUniforms(const std::vector<const void*>& data, ShaderType shaderType)
{
	if (shaderType == ShaderType::VERTEX)
	{
		for (std::size_t i = 0; i != data.size(); i++)
		{
			s_devcon->UpdateSubresource(uniformBuffers[i], 0, NULL, data[i], 0, 0);

			// take account of global constant buffer at register(b0)
			s_devcon->VSSetConstantBuffers(1 + i, 1, uniformBuffers[i].access());
		}
	}
	else if (shaderType == ShaderType::PIXEL)
	{
		for (std::size_t i = 0; i != data.size(); i++)
		{
			s_devcon->UpdateSubresource(uniformBuffers[i], 0, NULL, data[i], 0, 0);

			// take account of global constant buffer at register(b0)
			s_devcon->PSSetConstantBuffers(1 + i, 1, uniformBuffers[i].access());
		}
	}
	else
	{
		ERROR("Trying to bind uniform to wrong shader type");
	}
}

void engine::ShaderProgram::compileShader(const ShaderInfo& shader, ID3DBlob*& blob)
{
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
	flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	LPCSTR profile = (shader.type == ShaderType::PIXEL) ? "ps_5_0" : "vs_5_0";
	ID3DBlob* errorBlob = nullptr;
	HRESULT result = D3DCompileFromFile(shader.filePath, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, shader.funcName, profile, flags, NULL, &blob, &errorBlob);

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