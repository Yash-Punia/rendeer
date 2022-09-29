#include <iostream>
#include <string>
#include <chrono>
#include "rendermanager.h"
#include "shader.h"
#include "camera.h"
#include "glad/glad.h"
#include "sdl2/SDL.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include "engine.h"
#include "input/keyboard.h"
#include "glm/gtc/matrix_transform.hpp"

#define GL_CHECK() std::cout << glGetError() << std::endl;

namespace rendeer
{
    RenderManager::RenderManager() : mShader(nullptr), mCamera(nullptr)
    {
        Initialize();
    }
    RenderManager::~RenderManager()
    {
        Shutdown();
    }

    void RenderManager::Initialize()
    {
        // float vertices[] =
        //     {   // vertex coords    // Colours      // Texture coords
        //          0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        //          0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        //         -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        //         -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f};
        float vertices[] = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};
        unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3};

        // creating a vertex shader
        const std::string vertexShaderSource = R"(
            #version 410 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec2 aTexCoord;
            layout (location = 2) in vec3 aColor;

            out vec3 color;
            out vec2 texCoord;

            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;

            void main()
            {
                gl_Position = projection * view * model * vec4(aPos, 1.0);
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

            void main()
            {
                FragColor = texture(texture1, texCoord);
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

        unsigned char *data = stbi_load("D:\\Programming\\rendeer\\assets\\textures\\zdmn.png", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
            std::cout << "Error while loading image texture\n";

        stbi_image_free(data);

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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);


        // texture
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));

        // Unbinding the VAO and VBO
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);


        mCamera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    }

    void RenderManager::Render()
    {
        glClearColor(1.0f, 0.901f, 0.7255f, 1.0f);

        glm::vec3 cubePositions[] = {
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f, 2.0f, -2.5f),
            glm::vec3(1.5f, 0.2f, -1.5f),
            glm::vec3(-1.3f, 1.0f, -1.5f)};

        // tranformations

        glm::mat4 view = glm::mat4(1.0f);
        view = mCamera->GetViewMatrix();

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(mCamera->Zoom), Engine::GetWindowSize().x / Engine::GetWindowSize().y, 0.1f, 100.0f);

        mShader->SetUniformMat4fv("view", view);
        mShader->SetUniformMat4fv("projection", projection);

        mShader->Bind();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mTexture1);
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, mTexture2);

        glBindVertexArray(VAO);
        for (int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * (i + 1);
            model = glm::rotate(model, Engine::GetTicks() * glm::radians(angle), glm::vec3(1.0f, 0.5f, 0.3f));
            mShader->SetUniformMat4fv("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        mShader->Unbind();
    }

    void RenderManager::Update(float deltaTime)
    {
        if(mCamera)
        {
            mCamera->Update(deltaTime);
            if (input::Keyboard::GetKeyDown(RENDEER_INPUT_KEY_R))
                mCamera->ResetView();
        }
    }

    void RenderManager::Shutdown()
    {
        if (mShader) delete mShader;
        if (mCamera) delete mCamera;
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
} // namespace rendeer
