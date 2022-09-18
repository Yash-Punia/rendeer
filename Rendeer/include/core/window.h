#pragma once

struct SDL_Window;

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

    private:
        SDL_Window* mWindow;
    };
}