#include "../Maths/Maths.h"
#include "Core.h"

#include <string>

#ifndef SHADER_H
#define SHADER_H

namespace Cyclope {

    class API Shader
    {
    public:
        Shader(std::string path);
        Shader();
        ~Shader();

        unsigned int GetID() { return ID; }

        // use/activate the shader
        void Use();
        //delete the shader
        void Delete();

        // utility uniform functions
        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setColorValues3(const std::string& name, float r, float g, float b) const;
        void setColorValues4(const std::string& name, float r, float g, float b, float a) const;
        void setVec3(const std::string& name, float x, float y, float z) const;
        void setMat4(const std::string& name, const glm::mat4& mat) const;

    private:
        unsigned int ID;
    };

}

#endif // !SHADER_H
