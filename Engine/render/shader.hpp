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
		ID3DBlob* VS = nullptr;
		ID3DBlob* PS = nullptr;
		DxResPtr<ID3D11VertexShader> m_pVS;
		DxResPtr<ID3D11PixelShader> m_pPS;
		DxResPtr<ID3D11InputLayout> m_pLayout = nullptr;   
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_ied;

	public:
		std::vector<DxResPtr<ID3D11Buffer>> uniformBuffers;
		std::vector<UINT> m_sizes;

		ShaderProgram() = default;

		void init(std::vector<ShaderInfo> &shaders);
		void init(std::vector<ShaderInfo>& shaders, std::vector<D3D11_INPUT_ELEMENT_DESC>& ied);

		void clean();

		void createUniform(UINT size);
		void bindUniforms(const std::vector<const void*>& data, ShaderType = ShaderType::VERTEX);
		inline bool isUniformsEmpty() { return (uniformBuffers.size() == 0); }

		void compileShader(const ShaderInfo& shader, ID3DBlob*& blob);

		void bind();
		void unbind();
	};
}