#pragma once
#include <string>
#include "glad/glad.h"

namespace rendeer
{
    class Texture
    {
    public:
        Texture(std::string path);
        ~Texture();

        void Bind();
        void Bind(int textureIndex);
        void Unbind();
        
    private:
        unsigned int mId;
        int mWidth, mHeight, numChannels;
        unsigned char* pixels;

        void LoadTexture(std::string path);
        GLenum GetTextureFormat();
    };
} // namespace rendeer
