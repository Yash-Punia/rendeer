#include <iostream>
#include <string>
#include <chrono>
#include "rendermanager.h"
#include "shader.h"
#include "glad/glad.h"
#include "sdl2/SDL.h"
#include "stb_image.h"

#define GL_CHECK() std::cout << glGetError() << std::endl;

namespace rendeer
{
    RenderManager::RenderManager()
    {
        Initialize();
    }
    RenderManager::~RenderManager()
    {
        Shutdown();
    }

    void RenderManager::Initialize()
    {
        float vertices[] =
            {   // vertex coords    // Colours      // Texture coords
                 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f};

        unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3};

        // creating a vertex shader
        const std::string vertexShaderSource = R"(
            #version 410 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aColor;
            layout (location = 2) in vec2 aTexCoord;

            out vec3 color;
            out vec2 texCoord;

            void main()
            {
                gl_Position = vec4(aPos, 1.0);
                texCoord = aTexCoord;
                color = aColor;
            }
        )";

        const std::string fragmentShaderSource = R"(
            #version 410 core
            out vec4 FragColor;
            in vec3 color;
            in vec2 texCoord;

            uniform sampler2D texture1;
            uniform sampler2D texture2;

            void main()
            {
                FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.5);
            }
        )";


        glGenTextures(1, &mTexture1);

        glBindTexture(GL_TEXTURE_2D, mTexture1);

        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Loading the image using stb_image
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);  

        unsigned char* data = stbi_load("D:\\Programming\\rendeer\\assets\\textures\\container.jpg", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
            std::cout<<"Error while loading image texture\n";

        stbi_image_free(data);

        glGenTextures(1, &mTexture2);
        glBindTexture(GL_TEXTURE_2D, mTexture2);

        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_set_flip_vertically_on_load(true);  
        unsigned char* data2 = stbi_load("D:\\Programming\\rendeer\\assets\\textures\\zdmnLogo.png", &width, &height, &nrChannels, 0);
        if (data2)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        stbi_image_free(data2);

        mShader = new Shader(vertexShaderSource, fragmentShaderSource);

        // bind the VAO first, then bind the buffer and vertex attributes
        glGenVertexArrays(1, &VAO);

        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // moves the data from program to GPU

        glGenBuffers(1, &EBO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Linking vertex attributes
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

        // color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));

        // Unbinding the VAO and VBO
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);

        mShader->Bind();
        mShader->SetUniformInt("texture1", 0);
        mShader->SetUniformInt("texture2", 1);
        mShader->Unbind();

    }

    void RenderManager::Render()
    {
        glClearColor(0.3, 0.3, 0.3, 1);

        float timeValue = (float) SDL_GetTicks() / 1000.0f;
        float alphaValue = (SDL_sin(timeValue) / 2.0f) + 0.5f;
        mShader->Bind();
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mTexture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mTexture2);

        glBindVertexArray(VAO);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glBindVertexArray(0);

        mShader->Unbind();
    }

    void RenderManager::Shutdown()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
} // namespace rendeer
