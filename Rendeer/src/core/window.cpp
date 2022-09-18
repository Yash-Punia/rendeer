#include "core/window.h"
#include "sdl2/SDL.h"
#include "engine.h"

namespace rendeer
{
    Window::Window() : mWindow(nullptr) {}

    Window::~Window()
    {
        if (mWindow)
            Shutdown();
    }

    bool Window::Create()
    {
        mWindow = SDL_CreateWindow("Rendeer 🦌", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
        if (mWindow)
            return true;
        else
            return false;
    }

    void Window::Shutdown()
    {
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
    }

    void Window::PumpEvents()
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                Engine::Quit();
                break;
            
            default:
                break;
            }
        }
    }
}