#include "Cpch.h"
#include "Shader.h"

#include "Rendering.h"
#include "../API/OpenGL/OpenGLShader.h"

namespace Cyclope {

    GLenum TypeFromString(const std::string& str) {
        if (str == "vertex")
            return 1;
        else if (str == "fragment")
            return 2;
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

    Shared<Shader> Shader::Create(std::string path) {

        std::string vertexCode;
        std::string fragmentCode;

        std::ifstream shaderFile;

        std::unordered_map<GLenum, std::string> shaderSources;

        try {
            shaderFile.open(path);

            std::stringstream shaderStream;

            shaderStream << shaderFile.rdbuf();
            shaderSources = PreProcess(shaderStream.str());
            shaderFile.close();

            vertexCode = shaderSources[1];
            fragmentCode = shaderSources[2];
        }
        catch (std::ifstream::failure e) {
            std::cout << "[SHADER::FILE_NOT_SUCCESFULLY_READ]: " + std::string("--> Check location: ") + path << std::endl;
        }

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::OpenGL:
			return std::make_shared<OpenGLShader>(vShaderCode, fShaderCode);
			break;
		}

    }

}