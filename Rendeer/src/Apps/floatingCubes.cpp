#include "app.h"
#include "engine.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "input/keyboard.h"

#include "glm/glm.hpp"

#include <string>
#include <array>
#include <iostream>

using namespace rendeer;

class RendeerApp : public App
{
private:
    float *vertices;
    unsigned int *indices;
    Shader *shader;
    Mesh *mesh;
    Texture *texture;
    std::array<glm::vec3, 10> cubePositions;
    Camera *camera;

    // creating a vertex shader
    const std::string vertexShaderSource = R"(
            #version 410 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec2 aTexCoord;

            out vec2 texCoord;

            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;

            void main()
            {
                gl_Position = projection * view * model * vec4(aPos, 1.0);
                texCoord = aTexCoord;
            }
        )";

    const std::string fragmentShaderSource = R"(
            #version 410 core
            out vec4 FragColor;
            in vec2 texCoord;

            uniform sampler2D texture1;

            void main()
            {
                FragColor = texture(texture1, texCoord);
            }
        )";

public:
    void Initialize() override
    {
        float cubeVertices[] = {
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

        vertices = cubeVertices;

        glm::vec3 positions[] = {
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

        for(int i=0; i<10; i++)
            cubePositions[i] = positions[i];

        shader = new Shader(vertexShaderSource, fragmentShaderSource);
        texture = new Texture("D:\\Programming\\rendeer\\assets\\textures\\zdmn.png");
        mesh = new Mesh(vertices, sizeof(cubeVertices));

        camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    }

    void Shutdown() override
    {
        delete texture;
        delete shader;
        delete mesh;
        delete camera;
    }
    void Update(float deltaTime) override
    {
        if (camera)
        {
            camera->Update(deltaTime);
            if (input::Keyboard::GetKeyDown(RENDEER_INPUT_KEY_R))
                camera->ResetView();
        }
    }
    void Render() override
    {
        glClearColor(1.0f, 0.976f, 0.792f, 1.0f);

        texture->Bind();
        shader->Bind();
        mesh->Bind();

        glm::mat4 view = glm::mat4(0.0f);
        view = camera->GetViewMatrix();

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera->Zoom), Engine::GetWindowSize().x / Engine::GetWindowSize().y, 0.1f, 100.0f);

        shader->SetUniformMat4fv("view", view);
        shader->SetUniformMat4fv("projection", projection);

        for (int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * (i + 1);
            model = glm::rotate(model, Engine::GetTicks() * glm::radians(angle), glm::vec3(1.0f, 0.5f, 0.3f));
            shader->SetUniformMat4fv("model", model);
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        texture->Unbind();
        shader->Unbind();
        mesh->Unbind();

    }
};


// Uncomment below main function

// Make sure only one main function is there when building project

// int WinMain()
// {
//     App *app = new RendeerApp();

//     Engine::Create(app);
//     Engine::Run();
    
//     delete app;
    
//     return 0;
// }
