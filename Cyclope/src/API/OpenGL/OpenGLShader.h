#pragma once

#include "Core.h"

#include "../Rendering/Shader.h"

namespace Cyclope {

	class CYCLOPE_API OpenGLShader : public Shader {

	public:
		OpenGLShader(const char* vShaderCode, const char* fShaderCode);
		~OpenGLShader() override;

		void Bind() const override;
		void Unbind() const override;

		unsigned int GetID() const override;

		void SetBool(const std::string& name, bool value) const override;
		void SetInt(const std::string& name, int value) const override;
		void SetFloat(const std::string& name, float value) const override;
		void SetVec2(const std::string& name, const Vector2& vec) const override;
		void SetVec3(const std::string& name, const Vector3& vec) const override;
		void SetVec4(const std::string& name, const Vector4& vec) const override;
		void SetMat3(const std::string& name, const Matrix3& mat) const override;
		void SetMat4(const std::string& name, const Matrix4& mat) const override;

	private:
		unsigned int m_ID;

	};

}