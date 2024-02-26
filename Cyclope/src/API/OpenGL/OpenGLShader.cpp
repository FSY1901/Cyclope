#include "Cpch.h"
#include "OpenGLShader.h"

#include "glad.h"
#include "Log.h"

namespace Cyclope {

    OpenGLShader::OpenGLShader(const char* vShaderCode, const char* fShaderCode) {

        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);

        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            CYCLOPE_CORE_ERROR("[SHADER::VERTEX::COMPILATION_FAILED]:\n" + std::string(infoLog));
            return;
        };

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);

        int  success_;
        char infoLog_[512];

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success_);
        if (!success_)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog_);
            CYCLOPE_CORE_ERROR("[SHADER::FRAGMENT::COMPILATION_FAILED]:\n" + std::string(infoLog_));
            return;
        }

        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertex);
        glAttachShader(m_ID, fragment);
        glLinkProgram(m_ID);

        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
            CYCLOPE_CORE_ERROR("[SHADER::PROGRAM::LINKING_FAILED]:\n" + std::string(infoLog));
        }

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

    void OpenGLShader::SetMat3(const std::string& name, const Matrix3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void OpenGLShader::SetMat4(const std::string& name, const Matrix4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

}