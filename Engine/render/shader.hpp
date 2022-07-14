#pragma once

#include <string>
#include <vector>

#include "debug.hpp"
#include"DxRes.hpp"

enum class ShaderType { VERTEX, PIXEL};

struct ShaderInfo
{
	ShaderType type;
	LPCWSTR filePath;
	LPCSTR funcName;
};

namespace engine
{


	class ShaderProgram
	{
	private:
		ID3D10Blob* VS = nullptr;
		ID3D10Blob* PS = nullptr;
		DxResPtr<ID3D11VertexShader> m_pVS;
		DxResPtr<ID3D11PixelShader> m_pPS;
		DxResPtr<ID3D11InputLayout> m_pLayout;   
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_ied;


	public:
		std::vector<DxResPtr<ID3D11Buffer>> uniformBuffers;
		ShaderProgram() = default;

		void init(std::vector<ShaderInfo> &shaders);
		void init(std::vector<ShaderInfo>& shaders, std::vector<D3D11_INPUT_ELEMENT_DESC>& ied);

		void clean();

		void createUniform(UINT size);
		void bindUniforms(const std::vector<const void*>& data, ShaderType = ShaderType::VERTEX);

		void release();
		void compileShader(const ShaderInfo& shader, ID3D10Blob*& blob);

		void bind();
		void unbind();
	};
}