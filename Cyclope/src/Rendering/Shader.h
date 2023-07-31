#include "../Maths/Maths.h"
#include "Core.h"

#include <string>

#pragma once
namespace Cyclope {

    class CYCLOPE_API Shader
    {
    public:
        virtual ~Shader() {};

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual unsigned int GetID() const = 0;

        // utility uniform functions
        virtual void SetBool(const std::string& name, bool value) const = 0;
        virtual void SetInt(const std::string& name, int value) const = 0;
        virtual void SetFloat(const std::string& name, float value) const = 0;
        virtual void SetVec2(const std::string& name, const Vector2& vec) const = 0;
        virtual void SetVec3(const std::string& name, const Vector3& vec) const = 0;
        virtual void SetVec4(const std::string& name, const Vector4& vec) const = 0;
        virtual void SetMat4(const std::string& name, const Matrix4& mat) const = 0;

        static Shared<Shader> Create(std::string path);
    };

}
