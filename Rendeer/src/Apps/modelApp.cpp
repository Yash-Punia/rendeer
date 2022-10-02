#include "app.h"
#include "engine.h"
#include "mesh.h"
#include "model.h"
#include "shader.h"
#include "camera.h"
#include "input/keyboard.h"

#include "glm/glm.hpp"
#include "external/imgui/imgui.h"
#include "glad/glad.h"

#include <string>
#include <array>
#include <iostream>

using namespace rendeer;

class RendeerApp : public App
{
private:
    Camera *camera;
    bool updateCameraView;
    Model *model;
    Shader *shader;

    const std::string vertexShaderSource = R"(
            #version 410 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aNormal;
            layout (location = 2) in vec2 aTexCoord;

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
        updateCameraView = true;

        camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
        shader = new Shader(vertexShaderSource, fragmentShaderSource);
        model = new Model("D:\\Programming\\rendeer\\assets\\backpack.obj");
        // lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
    }

    void Shutdown() override
    {
        delete camera;
        delete shader;
        delete model;
    }
    void Update(float deltaTime) override
    {
        if (camera && updateCameraView)
        {
            camera->Update(deltaTime);
            if (input::Keyboard::GetKeyDown(RENDEER_INPUT_KEY_R))
                camera->ResetView();
        }

        if (input::Keyboard::GetKeyDown(RENDEER_INPUT_KEY_C))
            updateCameraView = !updateCameraView;
    }
    void Render() override
    {
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

        glm::vec2 windowSize = Engine::GetWindow().GetWindowSize();

        shader->Bind();

        glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), windowSize.x / windowSize.y, 0.1f, 100.0f);
        glm::mat4 view = camera->GetViewMatrix();
        shader->SetUniformMat4fv("projection", projection);
        shader->SetUniformMat4fv("view", view);

        model->Draw(shader);

        shader->Unbind();
    }

    void UI_Render() override
    {
    }
};

int WinMain()
{
    App *app = new RendeerApp();

    Engine::Create(app);
    Engine::Run();

    delete app;

    return 0;
}
