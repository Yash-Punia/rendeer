#pragma once

namespace rendeer
{
    class Shader;
    class RenderManager
    {
    public:
        RenderManager();
        ~RenderManager();

        void Render();
        void Initialize();
        void Shutdown();

    private:
        unsigned int VBO;
        unsigned int VAO;
        unsigned int EBO;
        unsigned int mTexture1;
        unsigned int mTexture2;
        Shader* mShader;
    };
} // namespace rendeer
