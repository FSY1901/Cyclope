#pragma once

#include "Core.h"

#include "glad.h"

#include <string>

namespace Cyclope {

	enum class CYCLOPE_API ImageFormat {
		R8,
		RGB8,
		RGBA8,
		RGBA32F
	};

	struct CYCLOPE_API TextureSpecification {
		int Width = 1;
		int Height = 1;
		ImageFormat Format = ImageFormat::RGBA8;
	};

	class CYCLOPE_API Texture2D {

	public:
		virtual ~Texture2D() {};
		virtual void Bind(unsigned int offset = 0) const = 0;
		virtual void Unbind(unsigned int offset = 0) const = 0;

		virtual void SetData(void* data) const = 0;

		virtual GLuint GetTexture() const = 0;
		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;


		static Shared<Texture2D> Create(const char* path);
		static Shared<Texture2D> Create(TextureSpecification spec, void* data);

	};

}