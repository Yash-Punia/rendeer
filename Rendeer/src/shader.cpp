#include <iostream>
#include "shader.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace rendeer
{
    Shader::Shader(const std::string& vertex, const std::string& fragment)
    {
        mShaderProgram = glCreateProgram();

        int status = GL_FALSE;
        char infoLog[512];

        unsigned int vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
        const char* glSource = vertex.c_str();        
        glShaderSource(vertexShaderId, 1, &glSource, NULL);
        glCompileShader(vertexShaderId);
        glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &status);

        if (status != GL_TRUE)
        {
            glGetShaderInfoLog(vertexShaderId, sizeof(infoLog), NULL, infoLog);
            std::cout<<"Vertex Shader compilation error: "<<infoLog<<"\n";
        }
        else
        {
            glAttachShader(mShaderProgram, vertexShaderId);
        }

        unsigned int fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
        glSource = fragment.c_str();
        glShaderSource(fragmentShaderId, 1, &glSource, NULL);
        glCompileShader(fragmentShaderId);
        glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &status);

        if (status != GL_TRUE)
        {
            glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
            std::cout<<"Fragment Shader compilation error: "<<infoLog<<"\n";
        }
        else
        {
            glAttachShader(mShaderProgram, fragmentShaderId);
        }

        glLinkProgram(mShaderProgram);
        glValidateProgram(mShaderProgram);
        
        glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
        if (!status)
        {
            glGetProgramInfoLog(mShaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                      << infoLog << std::endl;
            glDeleteProgram(mShaderProgram);
            mShaderProgram = -1;
        }
        
        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
    }

    Shader::~Shader()
    {
        glUseProgram(0);
        glDeleteProgram(mShaderProgram);
    }

    void Shader::Bind()
    {
        glUseProgram(mShaderProgram);
    }

    void Shader::Unbind()
    {
        glUseProgram(0);
    }

    void Shader::SetUniformInt(const std::string name, int val)
    {
        glUseProgram(mShaderProgram);
        glUniform1i(GetUniformLocation(name), val);
    }

    void Shader::SetUniformFloat(const std::string name, float val)
    {
        glUseProgram(mShaderProgram);
        glUniform1f(GetUniformLocation(name), val);
    }

    void Shader::SetUniformFloat2(const std::string name, float val1, float val2)
    {
        glUseProgram(mShaderProgram);
        glUniform2f(GetUniformLocation(name), val1, val2);
    }

    void Shader::SetUniformFloat3(const std::string name, float val1, float val2, float val3)
    {
        glUseProgram(mShaderProgram);
        glUniform3f(GetUniformLocation(name), val1, val2, val3);

    }

    void Shader::SetUniformFloat4(const std::string name, float val1, float val2, float val3, float val4)
    {
        glUseProgram(mShaderProgram);
        glUniform4f(GetUniformLocation(name), val1, val2, val3, val4);
    }

    void Shader::SetUniformMat4fv(const std::string name, const glm::mat4 matrix)
    {
        glUseProgram(mShaderProgram);
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    unsigned int Shader::GetUniformLocation(const std::string name)
    {
        auto location = mUniformLocations.find(name);

        if (location == mUniformLocations.end())
        {
            mUniformLocations[name] = glGetUniformLocation(mShaderProgram, name.c_str());
        }

        return mUniformLocations[name];
    }
} // namespace rendeer
