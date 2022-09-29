#pragma once
#include "glm/glm.hpp"

struct SDL_Window;
using SDL_GLContext = void*;

namespace rendeer
{
    class Window
    {
    public:
        Window();
        ~Window();

        bool Create();
        void Shutdown();

        // Queues for events
        void PumpEvents();

        void BeginRender();
        void EndRender();
        inline glm::vec2 GetWindowSize() { return glm::vec2(Width, Height); };
    private:
        const int Width = 1280;
        const int Height = 720;
        SDL_Window* mWindow;
        SDL_GLContext mGLContext;
    };
}