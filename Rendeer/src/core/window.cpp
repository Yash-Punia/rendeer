#include <iostream>
#include "core/window.h"
#include "sdl2/SDL.h"
#include "engine.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "glad/glad.h"

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
        mWindow = SDL_CreateWindow("Rendeer 🦌", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if (!mWindow)
            return false;

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_SetWindowMinimumSize(mWindow, 200, 200);

        mGLContext = SDL_GL_CreateContext(mWindow);

        if (mGLContext == nullptr)
        {
            std::cout<<"Error creating GL Context"<<std::endl;
        }

        // it maps all the function pointers to the graphic card
        gladLoadGLLoader(SDL_GL_GetProcAddress);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glEnable(GL_BLEND); // transparent object visibility
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

        return true;
    }

    void Window::Shutdown()
    {
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
    }

    void Window::BeginRender()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Window::EndRender()
    {
        SDL_GL_SwapWindow(mWindow); 
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
            
            case SDL_MOUSEWHEEL:
                input::Mouse::UpdateScroll(e.wheel.x, e.wheel.y);
                break;

            case SDL_KEYDOWN:
                if (SDLK_ESCAPE == e.key.keysym.sym)
                    Engine::Quit();
                break;
            }
        }

        if (e.type != SDL_MOUSEWHEEL)
            input::Mouse::UpdateScroll(0,0);

        input::Keyboard::Update();
        input::Mouse::Update();
    }
}