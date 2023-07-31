#include "OpenGLShader.h"

#include "glad.h"

#include <fstream>
#include <sstream>
#include <unordered_map>

namespace Cyclope {

    GLenum TypeFromString(const std::string& str) {
        if (str == "vertex")
            return GL_VERTEX_SHADER;
        else if (str == "fragment")
            return GL_FRAGMENT_SHADER;
    }

    std::unordered_map<GLenum, std::string> PreProcess(const std::string& source)
    {
        std::unordered_map<GLenum, std::string> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
            size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
            std::string type = source.substr(begin, eol - begin);

            size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
            pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

            shaderSources[TypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
        }

        return shaderSources;
    }

    OpenGLShader::OpenGLShader(std::string path) {

        //Store the Shader Code 
        std::string vertexCode;
        std::string fragmentCode;

        //the file in which the code is located
        std::ifstream shaderFile;

        std::unordered_map<GLenum, std::string> shaderSources;

        try {
            shaderFile.open(path);

            std::stringstream shaderStream;
            // reads the file and copies it to the string streams
            shaderStream << shaderFile.rdbuf();
            shaderSources = PreProcess(shaderStream.str());
            shaderFile.close();

            vertexCode = shaderSources[GL_VERTEX_SHADER];
            fragmentCode = shaderSources[GL_FRAGMENT_SHADER];
        }
        catch (std::ifstream::failure e) {
            std::cout << "[SHADER::FILE_NOT_SUCCESFULLY_READ]: " + std::string("--> Check location: ") + path << std::endl;
        }

        //convert to C-Strings (OpenGL needs it, because it's written in C)
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        ////////////////
        //COMPILING & LINKING
        ////////////////

        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        // vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // print compile errors if any
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "[SHADER::VERTEX::COMPILATION_FAILED]:\n" + std::string(infoLog) << std::endl;
            return;
        };

        //fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);

        int  success_;
        char infoLog_[512];
        //again, print compile errors
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success_);
        if (!success_)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog_);
            std::cout << "[SHADER::FRAGMENT::COMPILATION_FAILED]:\n" + std::string(infoLog_) << std::endl;
        }

        // shader Program & Linking
        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertex);
        glAttachShader(m_ID, fragment);
        glLinkProgram(m_ID);
        // print linking errors if any
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
            std::cout << "[SHADER::PROGRAM::LINKING_FAILED]:\n" + std::string(infoLog) << std::endl;
        }

        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);

    }

    OpenGLShader::~OpenGLShader() {
        glDeleteProgram(m_ID);
    }

    void OpenGLShader::Bind() const{
        glUseProgram(m_ID);
    }

    void OpenGLShader::Unbind() const {
        glUseProgram(0);
    }

    unsigned int OpenGLShader::GetID() const { return m_ID; }

    void OpenGLShader::SetBool(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
    }

    void OpenGLShader::SetInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
    }

    void OpenGLShader::SetFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
    }

    void OpenGLShader::SetVec4(const std::string& name, const Vector4& vec) const {
        glUniform4f(glGetUniformLocation(m_ID, name.c_str()), vec.x, vec.y, vec.z, vec.w);
    }

    void OpenGLShader::SetVec2(const std::string& name, const Vector2& vec) const {
        glUniform2f(glGetUniformLocation(m_ID, name.c_str()), vec.x, vec.y);
    }

    void OpenGLShader::SetVec3(const std::string& name, const Vector3& vec) const {
        glUniform3f(glGetUniformLocation(m_ID, name.c_str()), vec.x, vec.y, vec.z);
    }

    void OpenGLShader::SetMat4(const std::string& name, const Matrix4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

}