#pragma once

namespace rendeer
{
    class Shader;
    class Camera;
    class RenderManager
    {
    public:
        RenderManager();
        ~RenderManager();

        void Render();
        void Update(float);
        void Initialize();
        void Shutdown();

    private:
        unsigned int VBO;
        unsigned int VAO;
        unsigned int EBO;
        unsigned int mTexture1;
        unsigned int mTexture2;
        Shader* mShader;
        Camera* mCamera;
    };
} // namespace rendeer
