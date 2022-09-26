#include "rendermanager.h"
#include "shader.h"
#include <iostream>
#include <string>
#include "glad/glad.h"
#include "sdl2/SDL.h"
#include <chrono>

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
            {
                0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
                -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f};

        unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3};

        // creating a vertex shader
        const std::string vertexShaderSource = R"(
            #version 410 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aColor;

            out vec3 color;

            void main()
            {
                gl_Position = vec4(aPos, 1.0);
                color = aColor;
            }
        )";

        const std::string fragmentShaderSource = R"(
            #version 410 core
            out vec4 FragColor;
            in vec3 color;
            void main()
            {
                FragColor = vec4(color, 1.0f);
            }
        )";

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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

        // color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));

        // Unbinding the VAO and VBO
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }

    void RenderManager::Render()
    {
        glClearColor(0.3, 0.3, 0.3, 1);

        float timeValue = (float) SDL_GetTicks() / 1000.0f;
        float blueValue = (SDL_sin(timeValue) / 2.0f) + 0.5f;
        mShader->Bind();
        mShader->SetUniformFloat4("color", 0.0f, 0.0f, blueValue, 1.0f);

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
