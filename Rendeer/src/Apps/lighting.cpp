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
    Shader *lightBoxShader, *boxShader;
    Mesh *box;
    Mesh *lightBox;
    Camera *camera;
    glm::vec3 lightPos;

    // BOX SHADERS
    const std::string boxVertexShaderSource = R"(
            #version 410 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aNormal;

            out vec3 Normal;
            out vec3 FragPos;

            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;

            void main()
            {
                gl_Position = projection * view * model * vec4(aPos, 1.0);
                FragPos = vec3(model * vec4(aPos, 1.0));
                Normal = aNormal;
            }
        )";

    const std::string boxFragmentShaderSource = R"(
            #version 410 core
            out vec4 FragColor;

            in vec3 Normal;
            in vec3 FragPos;

            uniform vec3 objectColor;
            uniform vec3 lightColor;
            uniform vec3 lightPos;
            uniform vec3 viewPos;

            void main()
            {
                float ambientStrength = 0.1;
                vec3 ambient = ambientStrength * lightColor;

                vec3 norm = normalize(Normal);
                vec3 lightDir = normalize(lightPos - FragPos);
                float diff = max(dot(norm, lightDir), 0.0);
                vec3 diffuse = diff * lightColor;

                float specularStrength = 0.5;
                vec3 viewDir = normalize(viewPos - FragPos);
                vec3 reflectDir = reflect(-lightDir, norm);
                float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
                vec3 specular = specularStrength * spec * lightColor;

                vec3 result = (ambient + diffuse + specular) * objectColor;
                FragColor = vec4(result, 1.0);
            }
        )";

    
    //////////// LIGHT BOX SHADERS 

    const std::string lightBoxVertexShaderSource = R"(
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

    const std::string lightBoxFragmentShaderSource = R"(
            #version 410 core
            out vec4 FragColor;

            void main()
            {
                FragColor = vec4(1.0f);
            }
        )";

public:
    void Initialize() override
    {
        float lightBoxVertices[] = {
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            -0.5f, 0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f, -0.5f, 0.5f,

            -0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,

            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,

            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, -0.5f};

        float boxVertices[] = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f};

        int boxAttributes[] = {3,3};
        int lightBoxAttributes[] = {3};

        box = new Mesh(boxVertices, sizeof(boxVertices), boxAttributes, 2);
        boxShader = new Shader(boxVertexShaderSource, boxFragmentShaderSource);
        
        lightBox = new Mesh(lightBoxVertices, sizeof(lightBoxVertices), lightBoxAttributes, 1);
        lightBoxShader = new Shader(lightBoxVertexShaderSource, lightBoxFragmentShaderSource);

        camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

        lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
    }

    void Shutdown() override
    {
        delete boxShader;
        delete lightBoxShader;
        delete box;
        delete lightBox;
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
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

        glm::mat4 view = glm::mat4(1.0f);
        view = camera->GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), Engine::GetWindowSize().x / Engine::GetWindowSize().y, 0.1f, 100.0f);

        box->Bind();
        boxShader->Bind();

        boxShader->SetUniformFloat3("objectColor", 1.0, 0.5f, 0.31f);
        boxShader->SetUniformFloat3("lightColor", 1.0f, 1.0f, 1.0f);
        boxShader->SetUniformFloat3("lightPos", lightPos.x, lightPos.y, lightPos.z);
        boxShader->SetUniformFloat3("viewPos", camera->Position.x, camera->Position.y, camera->Position.z);

        boxShader->SetUniformMat4fv("view", view);
        boxShader->SetUniformMat4fv("projection", projection);

        glm::mat4 model = glm::mat4(1.0f);
        boxShader->SetUniformMat4fv("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        boxShader->Unbind();
        box->Unbind();

        lightBox->Bind();
        lightBoxShader->Bind();

        lightBoxShader->SetUniformMat4fv("view", view);
        lightBoxShader->SetUniformMat4fv("projection", projection);

        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightBoxShader->SetUniformMat4fv("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        lightBoxShader->Unbind();
        lightBox->Unbind();
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
