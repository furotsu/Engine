#pragma once

#include <string>
#include <iostream>
#include "DxRes.hpp"

namespace engine
{
	enum class TextureType
	{
		TEXTURE_DEFAULT, TEXTURE_CUBEMAP
	};

	class Texture
	{
	public:
		DxResPtr<ID3D11ShaderResourceView> pRView;

		Texture() = default;

		void init(const wchar_t* filepath, TextureType type = TextureType::TEXTURE_DEFAULT);
		void clean();

		void bind();
		void unbind();

	};
}