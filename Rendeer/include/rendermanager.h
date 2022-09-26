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
        Shader* mShader;
    };
} // namespace rendeer
