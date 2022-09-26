#pragma once

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

    private:
        SDL_Window* mWindow;
        SDL_GLContext mGLContext;
    };
}