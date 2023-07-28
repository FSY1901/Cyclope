#include "../Maths/Maths.h"
#include "Core.h"

#include <string>

#ifndef SHADER_H
#define SHADER_H

namespace Cyclope {

    class CYCLOPE_API Shader
    {
    public:
        Shader();
        ~Shader();

        unsigned int GetID() { return ID; }

        void Create(std::string path);
        void Use();
        void Delete();

        // utility uniform functions
        void SetBool(const std::string& name, bool value) const;
        void SetInt(const std::string& name, int value) const;
        void SetFloat(const std::string& name, float value) const;
        void SetVec2(const std::string& name, const Vector2& vec) const;
        void SetVec3(const std::string& name, const Vector3& vec) const;
        void SetVec4(const std::string& name, const Vector4& vec) const;
        void SetMat4(const std::string& name, const Matrix4& mat) const;

    private:
        unsigned int ID;
    };

}

#endif // !SHADER_H
