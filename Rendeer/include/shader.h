#pragma once
#include <string>
#include <unordered_map>

namespace rendeer
{
    class Shader
    {
    public:
        Shader(const std::string& vertex, const std::string& fragment);
        ~Shader();

        void Bind();
        void Unbind();

        void SetUniformInt(const std::string, int val);
        void SetUniformFloat(const std::string, float val);
        void SetUniformFloat2(const std::string, float val1, float val2);
        void SetUniformFloat3(const std::string, float val1, float val2, float val3);
        void SetUniformFloat4(const std::string, float val1, float val2, float val3, float val4);

    private:
        unsigned int mShaderProgram;
        std::unordered_map<std::string, int> mUniformLocations; 

        unsigned int GetUniformLocation(const std::string name);
    };
} // namespace rendeer
