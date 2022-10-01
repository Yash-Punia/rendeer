#include "texture.h"
#include "glad/glad.h"
#include "stb_image.h"
#include <string>
#include <iostream>

namespace rendeer
{
    Texture::Texture(std::string path)
    {
        glGenTextures(1, &mId);

        glBindTexture(GL_TEXTURE_2D, mId);

        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        LoadTexture(path);
    }

    Texture::~Texture()
    {
        
    }

    void Texture::Bind()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mId);
    }

    void Texture::Bind(int textureIndex)
    {
        glActiveTexture(GL_TEXTURE0 + textureIndex);
        glBindTexture(GL_TEXTURE_2D, mId);
    }
   
    void Texture::Unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::LoadTexture(std::string path)
    {
        stbi_set_flip_vertically_on_load(true);

        pixels = stbi_load(path.c_str(), &mWidth, &mHeight, &numChannels, 0);
        if (pixels)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GetTextureFormat(), mWidth, mHeight, 0, GetTextureFormat(), GL_UNSIGNED_BYTE, pixels);
            glGenerateMipmap(GL_TEXTURE_2D);
            std::cout << "Texture loaded\n";
        }
        else
            std::cout << "Error while loading image texture\n";
        stbi_image_free(pixels);
    }

    GLenum Texture::GetTextureFormat()
    {
        if (pixels)
        {
            switch (numChannels)
            {
            case 3:
                return GL_RGB;
                break;

            case 4:
                return GL_RGBA;
                break;

            default:
                break;
            }
        }
        return GL_RGB;
    }
} // namespace rendeer
