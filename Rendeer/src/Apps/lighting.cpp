// #include "app.h"
// #include "engine.h"
// #include "mesh.h"
// #include "shader.h"
// #include "texture.h"
// #include "camera.h"
// #include "input/keyboard.h"

// #include "glm/glm.hpp"
// #include "external/imgui/imgui.h"

// #include <string>
// #include <array>
// #include <iostream>

// using namespace rendeer;

// class RendeerApp : public App
// {
// private:
//     Shader *lightBoxShader, *boxShader;
//     Mesh *box;
//     Mesh *lightBox;
//     Camera *camera;
//     Texture *containerTexture, *specTexture;
//     glm::vec3 lightPos;
//     bool updateCameraView;

//     // BOX SHADERS
//     const std::string boxVertexShaderSource = R"(
//             #version 410 core
//             layout (location = 0) in vec3 aPos;
//             layout (location = 1) in vec3 aNormal;
//             layout (location = 2) in vec2 aTexCoords;

//             out vec3 Normal;
//             out vec3 FragPos;
//             out vec2 TexCoords;

//             uniform mat4 model;
//             uniform mat4 view;
//             uniform mat4 projection;

//             void main()
//             {
//                 gl_Position = projection * view * model * vec4(aPos, 1.0);
//                 FragPos = vec3(model * vec4(aPos, 1.0));
//                 Normal = aNormal;
//                 TexCoords = aTexCoords;
//             }
//         )";

//     const std::string boxFragmentShaderSource = R"(
//             #version 410 core
//             out vec4 FragColor;

//             struct Material {
//                 sampler2D diffuse;
//                 sampler2D specular;
//                 float shininess;
//             };

//             struct Light {
//                 vec3 position;
                
//                 vec3 ambient;
//                 vec3 diffuse;
//                 vec3 specular;
//             };

//             in vec3 Normal;
//             in vec3 FragPos;
//             in vec2 TexCoords;

//             uniform vec3 viewPos;
//             uniform Material material;
//             uniform Light light;

//             void main()
//             {
//                 vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

//                 vec3 norm = normalize(Normal);
//                 vec3 lightDir = normalize(light.position - FragPos);
//                 float diff = max(dot(norm, lightDir), 0.0);
//                 vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

//                 vec3 viewDir = normalize(viewPos - FragPos);
//                 vec3 reflectDir = reflect(-lightDir, norm);
//                 float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//                 vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoords));

//                 vec3 result = ambient + diffuse + specular;
//                 FragColor = vec4(result, 1.0);
//             }
//         )";

//     //////////// LIGHT BOX SHADERS

//     const std::string lightBoxVertexShaderSource = R"(
//             #version 410 core
//             layout (location = 0) in vec3 aPos;
//             layout (location = 1) in vec2 aTexCoord;

//             out vec2 texCoord;

//             uniform mat4 model;
//             uniform mat4 view;
//             uniform mat4 projection;

//             void main()
//             {
//                 gl_Position = projection * view * model * vec4(aPos, 1.0);
//                 texCoord = aTexCoord;
//             }
//         )";

//     const std::string lightBoxFragmentShaderSource = R"(
//             #version 410 core
//             out vec4 FragColor;

//             void main()
//             {
//                 FragColor = vec4(1.0f);
//             }
//         )";

// public:
//     void Initialize() override
//     {
//         updateCameraView = true;
//         float lightBoxVertices[] = {
//             -0.5f, -0.5f, -0.5f,
//             0.5f, -0.5f, -0.5f,
//             0.5f, 0.5f, -0.5f,
//             0.5f, 0.5f, -0.5f,
//             -0.5f, 0.5f, -0.5f,
//             -0.5f, -0.5f, -0.5f,

//             -0.5f, -0.5f, 0.5f,
//             0.5f, -0.5f, 0.5f,
//             0.5f, 0.5f, 0.5f,
//             0.5f, 0.5f, 0.5f,
//             -0.5f, 0.5f, 0.5f,
//             -0.5f, -0.5f, 0.5f,

//             -0.5f, 0.5f, 0.5f,
//             -0.5f, 0.5f, -0.5f,
//             -0.5f, -0.5f, -0.5f,
//             -0.5f, -0.5f, -0.5f,
//             -0.5f, -0.5f, 0.5f,
//             -0.5f, 0.5f, 0.5f,

//             0.5f, 0.5f, 0.5f,
//             0.5f, 0.5f, -0.5f,
//             0.5f, -0.5f, -0.5f,
//             0.5f, -0.5f, -0.5f,
//             0.5f, -0.5f, 0.5f,
//             0.5f, 0.5f, 0.5f,

//             -0.5f, -0.5f, -0.5f,
//             0.5f, -0.5f, -0.5f,
//             0.5f, -0.5f, 0.5f,
//             0.5f, -0.5f, 0.5f,
//             -0.5f, -0.5f, 0.5f,
//             -0.5f, -0.5f, -0.5f,

//             -0.5f, 0.5f, -0.5f,
//             0.5f, 0.5f, -0.5f,
//             0.5f, 0.5f, 0.5f,
//             0.5f, 0.5f, 0.5f,
//             -0.5f, 0.5f, 0.5f,
//             -0.5f, 0.5f, -0.5f};

//         float boxVertices[] = {
//             // positions          // normals           // texture coords
//             -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
//             0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
//             0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
//             0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
//             -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
//             -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

//             -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//             0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
//             0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
//             0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
//             -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
//             -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

//             -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
//             -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
//             -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
//             -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
//             -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
//             -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

//             0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
//             0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
//             0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
//             0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
//             0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
//             0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

//             -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
//             0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
//             0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
//             0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
//             -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
//             -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

//             -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
//             0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
//             0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
//             0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
//             -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
//             -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

//         int boxAttributes[] = {3, 3, 2};
//         int lightBoxAttributes[] = {3};

//         box = new Mesh(boxVertices, sizeof(boxVertices), boxAttributes, 3);
//         boxShader = new Shader(boxVertexShaderSource, boxFragmentShaderSource);

//         lightBox = new Mesh(lightBoxVertices, sizeof(lightBoxVertices), lightBoxAttributes, 1);
//         lightBoxShader = new Shader(lightBoxVertexShaderSource, lightBoxFragmentShaderSource);

//         camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

//         containerTexture = new Texture("D:\\Programming\\rendeer\\assets\\textures\\container2.png");
//         specTexture = new Texture("D:\\Programming\\rendeer\\assets\\textures\\container2_specular.png");

//         lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
//     }

//     void Shutdown() override
//     {
//         delete boxShader;
//         delete lightBoxShader;
//         delete box;
//         delete lightBox;
//         delete camera;
//     }
//     void Update(float deltaTime) override
//     {
//         if (camera && updateCameraView)
//         {
//             camera->Update(deltaTime);
//             if (input::Keyboard::GetKeyDown(RENDEER_INPUT_KEY_R))
//                 camera->ResetView();
//         }

//         if (input::Keyboard::GetKeyDown(RENDEER_INPUT_KEY_C))
//             updateCameraView = !updateCameraView;
//     }
//     void Render() override
//     {
//         glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

//         glm::mat4 view = glm::mat4(1.0f);
//         view = camera->GetViewMatrix();
//         glm::vec2 windowSize = Engine::GetWindow().GetWindowSize();
//         glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), windowSize.x / windowSize.y, 0.1f, 100.0f);

//         box->Bind();
//         boxShader->Bind();
//         containerTexture->Bind(0);
//         specTexture->Bind(1);

//         boxShader->SetUniformFloat3("light.position", lightPos.x, lightPos.y, lightPos.z);
//         boxShader->SetUniformFloat3("viewPos", camera->Position.x, camera->Position.y, camera->Position.z);
//         boxShader->SetUniformFloat3("material.ambient", 1.0f, 0.5f, 0.31f);
//         boxShader->SetUniformFloat3("material.diffuse", 1.0f, 0.5f, 0.31f);
//         boxShader->SetUniformFloat3("material.specular", 0.5f, 0.5f, 0.5f);
//         boxShader->SetUniformFloat("material.shininess", 32.0f);
//         boxShader->SetUniformFloat3("light.ambient", 0.2f, 0.2f, 0.2f);
//         boxShader->SetUniformFloat3("light.diffuse", 0.5f, 0.5f, 0.5f);
//         boxShader->SetUniformFloat3("light.specular", 1.0f, 1.0f, 1.0f);

//         boxShader->SetUniformInt("material.diffuse", 0);
//         boxShader->SetUniformInt("material.specular", 1);

//         boxShader->SetUniformMat4fv("view", view);
//         boxShader->SetUniformMat4fv("projection", projection);

//         glm::mat4 model = glm::mat4(1.0f);
//         boxShader->SetUniformMat4fv("model", model);

//         glDrawArrays(GL_TRIANGLES, 0, 36);

//         specTexture->Unbind();
//         containerTexture->Unbind();
//         boxShader->Unbind();
//         box->Unbind();

//         lightBox->Bind();
//         lightBoxShader->Bind();
//         lightBoxShader->SetUniformMat4fv("view", view);
//         lightBoxShader->SetUniformMat4fv("projection", projection);

//         model = glm::translate(model, lightPos);
//         model = glm::scale(model, glm::vec3(0.2f));
//         lightBoxShader->SetUniformMat4fv("model", model);

//         glDrawArrays(GL_TRIANGLES, 0, 36);

//         lightBoxShader->Unbind();
//         lightBox->Unbind();
//     }

//     void UI_Render() override
//     {
//         if (ImGui::Begin("Light Position"))
//         {
//             ImGui::DragFloat("Light Pos X", &lightPos.x, 0.1f);
//         }
//         ImGui::End();
//     }
// };

// // int WinMain()
// // {
// //     App *app = new RendeerApp();

// //     Engine::Create(app);
// //     Engine::Run();

// //     delete app;

// //     return 0;
// // }
